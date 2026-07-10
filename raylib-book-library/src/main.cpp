#include "raylib.h"
#include "cslinklist"
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <algorithm>
using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    int year;
    string genre;
    Book() : id(0), year(0) {}
    Book(int i, string t, string a, int y, string g) : id(i), title(t), author(a), year(y), genre(g) {}
    bool operator==(const Book& o) const { return id == o.id; }
};

const char* FILE_PATH = "books.csv";

string csvEscape(const string& field) {
    bool needsQuotes = field.find(',') != string::npos ||
                        field.find('"') != string::npos ||
                        field.find('\n') != string::npos;
    if (!needsQuotes) return field;
    string out = "\"";
    for (char c : field) {
        if (c == '"') out += "\"\"";
        else out += c;
    }
    out += "\"";
    return out;
}

vector<string> csvParseLine(const string& line) {
    vector<string> fields;
    string cur;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (inQuotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') { cur += '"'; i++; }
                else inQuotes = false;
            } else cur += c;
        } else {
            if (c == '"') inQuotes = true;
            else if (c == ',') { fields.push_back(cur); cur.clear(); }
            else cur += c;
        }
    }
    fields.push_back(cur);
    return fields;
}

void saveToFile(const SLinkList<Book>& list) {
    ofstream f(FILE_PATH);
    if (!f) return;
    for (int i = 0; i < list.size(); i++) {
        Book b = list.get(i);
        f << csvEscape(to_string(b.id)) << ","
          << csvEscape(b.title) << ","
          << csvEscape(b.author) << ","
          << csvEscape(to_string(b.year)) << ","
          << csvEscape(b.genre) << "\n";
    }
}

void loadFromFile(SLinkList<Book>& list) {
    ifstream f(FILE_PATH);
    if (!f) return;
    string line;
    while (getline(f, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        vector<string> fields = csvParseLine(line);
        if (fields.size() < 5) continue;
        int id = fields[0].empty() ? 0 : stoi(fields[0]);
        string title = fields[1];
        string author = fields[2];
        int year = fields[3].empty() ? 0 : stoi(fields[3]);
        string genre = fields[4];
        list.pushBack(Book(id, title, author, year, genre));
    }
}

string lower(const string& s) {
    string r = s;
    for (char& c : r) c = (char)tolower((unsigned char)c);
    return r;
}

bool matches(const Book& b, const string& lq) {
    if (lq.empty()) return true;
    return lower(to_string(b.id)).find(lq) != string::npos ||
           lower(b.title).find(lq) != string::npos ||
           lower(b.author).find(lq) != string::npos ||
           lower(to_string(b.year)).find(lq) != string::npos ||
           lower(b.genre).find(lq) != string::npos;
}

// ---- Global state ----
SLinkList<Book> books;
vector<int> filtered;
int selFiltered = -1;
int scrollOffset = 0;
string searchText;
bool searchFocused = false;
Font customFont;
float scale = 1.25f;

// ---- Base layout constants (multiplied by scale at runtime) ----
const int BASE_W = 860;
const int BASE_H = 560;
const int BASE_MARGIN = 8;
const int BASE_BTN_H = 28;
const int BASE_BTN_W = 60;
const int BASE_HEADER_H = 22;
const int BASE_ROW_H = 26;
const int BASE_STATUSBAR_H = 25;
const int BASE_BTN_FS = 16;
const int BASE_COL_W[] = { 60, 310, 260, 65, 145 };

int W, H, MARGIN, BTN_H, BTN_W, HEADER_H, ROW_H, STATUSBAR_H, FONT_SIZE;
int COL_W[5];

const char* COL_NAMES[] = { "ID", "Title", "Author", "Year", "Genre" };

Rectangle searchRect, addBtn, editBtn, delBtn, countRect, listRect, statusRect;
int colX[5];

// ---- Dialog state ----
enum AppState { MAIN, ADD_DIALOG, EDIT_DIALOG, CONFIRM_DELETE };
AppState state = MAIN;

const char* dlgLabels[5] = { "ID:", "Title:", "Author:", "Year:", "Genre:" };
string dlgVal[5];
int dlgActive = 0;
int dlgEditIdx = -1;

string confirmMsg;
int confirmIdx = -1;

// Double-click
double lastClickTime = 0;
int lastClickRow = -1;

inline int s(int v) { return (int)(v * scale + 0.5f); }

void ApplyScale() {
    W = s(BASE_W);
    H = s(BASE_H);
    MARGIN = s(BASE_MARGIN);
    BTN_H = s(BASE_BTN_H);
    BTN_W = s(BASE_BTN_W);
    HEADER_H = s(BASE_HEADER_H);
    ROW_H = s(BASE_ROW_H);
    STATUSBAR_H = s(BASE_STATUSBAR_H);
    FONT_SIZE = s(BASE_BTN_FS);
    for (int i = 0; i < 5; i++)
        COL_W[i] = s(BASE_COL_W[i]);
}

// ---- Helpers ----
string trunc(const string& s, int maxW, int fs) {
    if (MeasureTextEx(customFont, s.c_str(), (float)fs, 1).x <= maxW) return s;
    string r = s;
    while (!r.empty() && MeasureTextEx(customFont, (r + "...").c_str(), (float)fs, 1).x > maxW)
        r.pop_back();
    return r + "...";
}

void SetupLayout() {
    int ty = MARGIN;
    searchRect = { (float)MARGIN, (float)ty, 200.0f * scale, (float)BTN_H };
    int bx = (int)searchRect.x + (int)(200.0f * scale) + MARGIN;
    addBtn = { (float)bx, (float)ty, (float)BTN_W, (float)BTN_H };
    editBtn = { (float)(bx + BTN_W + s(4)), (float)ty, (float)BTN_W, (float)BTN_H };
    delBtn = { (float)(bx + 2 * (BTN_W + s(4))), (float)ty, (float)BTN_W, (float)BTN_H };
    countRect = { (float)(delBtn.x + delBtn.width + MARGIN), (float)ty, 200.0f * scale, (float)BTN_H };

    int listY = ty + BTN_H + MARGIN;
    int listH = H - listY - STATUSBAR_H - MARGIN;
    listRect = { (float)MARGIN, (float)listY, (float)(W - 2 * MARGIN), (float)listH };

    int cx = (int)listRect.x;
    for (int i = 0; i < 5; i++) {
        colX[i] = cx;
        cx += COL_W[i];
    }

    statusRect = { (float)MARGIN, (float)(H - STATUSBAR_H - MARGIN), (float)(W - 2 * MARGIN), (float)STATUSBAR_H };
}

void RebuildFilter() {
    string q = lower(searchText);
    filtered.clear();
    for (int i = 0; i < books.size(); i++)
        if (matches(books.get(i), q))
            filtered.push_back(i);
    if (selFiltered >= (int)filtered.size())
        selFiltered = (int)filtered.size() - 1;
    int maxRows = (int)(listRect.height / ROW_H);
    int maxSc = max(0, (int)filtered.size() - maxRows);
    if (scrollOffset > maxSc)
        scrollOffset = maxSc;
}

int VisibleRows() { return (int)(listRect.height / ROW_H); }
int MaxScroll() { return max(0, (int)filtered.size() - VisibleRows()); }

int RowFromMouse(Vector2 mp) {
    if (!CheckCollisionPointRec(mp, listRect)) return -1;
    int row = (int)((mp.y - listRect.y) / ROW_H) + scrollOffset;
    if (row < 0 || row >= (int)filtered.size()) return -1;
    return row;
}

// ---- Drawing ----
const Color CLR_PRIMARY = { 0, 103, 192, 255 };
const Color CLR_SUCCESS = { 40, 167, 69, 255 };
const Color CLR_WARNING = { 230, 162, 30, 255 };
const Color CLR_DANGER = { 220, 53, 69, 255 };
const Color CLR_SECONDARY = { 108, 117, 125, 255 };

Color Darker(Color c) {
    return { (unsigned char)max(0, (int)c.r - 30),
             (unsigned char)max(0, (int)c.g - 30),
             (unsigned char)max(0, (int)c.b - 30), 255 };
}

void DrawButton(Rectangle r, const char* text, bool enabled, bool hover, Color accent) {
    Color bg;
    if (!enabled) bg = { 200, 200, 200, 255 };
    else if (hover) bg = Darker(accent);
    else bg = accent;
    DrawRectangleRec(r, bg);
    DrawRectangleLinesEx(r, 1, Darker(accent));
    int fs = FONT_SIZE;
    Vector2 ts = MeasureTextEx(customFont, text, (float)fs, 1);
    int tw = (int)ts.x;
    DrawTextEx(customFont, text, { r.x + (r.width - tw) / 2, r.y + (r.height - fs) / 2 }, (float)fs, 1, WHITE);
}

void DrawInputField(Rectangle r, const string& text, bool active, const char* hint) {
    DrawRectangleRec(r, WHITE);
    DrawRectangleLinesEx(r, active ? 2.0f : 1.0f, active ? SKYBLUE : (Color){ 180, 180, 180, 255 });
    int fs = FONT_SIZE;
    if (text.empty() && !active)
        DrawTextEx(customFont, hint, { r.x + 4, r.y + (r.height - fs) / 2 }, (float)fs, 1, { 180, 180, 180, 255 });
    else
        DrawTextEx(customFont, text.c_str(), { r.x + 4, r.y + (r.height - fs) / 2 }, (float)fs, 1, BLACK);
    if (active && ((int)(GetTime() * 2) % 2) == 0) {
        Vector2 cs = MeasureTextEx(customFont, text.c_str(), (float)fs, 1);
        int cx = (int)r.x + 4 + (int)cs.x + 1;
        DrawRectangle(cx, (int)r.y + 4, 2, (int)r.height - 8, BLACK);
    }
}

void HandleTextInput(string& text, int maxLen) {
    int c = GetCharPressed();
    while (c > 0) {
        if (c >= 32 && c < 127 && (int)text.size() < maxLen)
            text += (char)c;
        c = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !text.empty())
        text.pop_back();
}

void DrawMain() {
    ClearBackground(WHITE);

    Vector2 mp = GetMousePosition();

    DrawInputField(searchRect, searchText, searchFocused, "Search...");
    DrawButton(addBtn, "Add", true, CheckCollisionPointRec(mp, addBtn), CLR_PRIMARY);
    DrawButton(editBtn, "Edit", selFiltered >= 0, CheckCollisionPointRec(mp, editBtn), CLR_WARNING);
    DrawButton(delBtn, "Delete", selFiltered >= 0, CheckCollisionPointRec(mp, delBtn), CLR_DANGER);

    string cl = to_string(filtered.size()) + " book(s)";
    DrawTextEx(customFont, cl.c_str(), { (float)countRect.x, (float)countRect.y + 6 }, (float)FONT_SIZE, 1, (Color){ 100, 100, 100, 255 });

    // Header
    DrawRectangle((int)listRect.x, (int)listRect.y, (int)listRect.width, HEADER_H, { 0, 103, 192, 255 });
    for (int i = 0; i < 5; i++) {
        if (i > 0)
            DrawLine(colX[i], (int)listRect.y, colX[i], (int)listRect.y + HEADER_H, { 0, 82, 165, 255 });
        DrawTextEx(customFont, COL_NAMES[i], { (float)(colX[i] + 6), (float)((int)listRect.y + 3) }, (float)FONT_SIZE, 1, WHITE);
    }

    // Rows
    int vr = VisibleRows();
    int drawY = (int)listRect.y + HEADER_H;
    DrawRectangle((int)listRect.x, drawY, (int)listRect.width, vr * ROW_H, WHITE);

    for (int i = scrollOffset; i < (int)filtered.size() && i < scrollOffset + vr; i++) {
        int ry = drawY + (i - scrollOffset) * ROW_H;
        Color rowBg = WHITE;
        if (i == selFiltered) rowBg = { 210, 228, 245, 255 };
        else if ((i - scrollOffset) % 2 == 1) rowBg = { 245, 248, 252, 255 };

        DrawRectangle((int)listRect.x + 1, ry, (int)listRect.width - 2, ROW_H, rowBg);

        Book b = books.get(filtered[i]);
        string fs[5] = { to_string(b.id), b.title, b.author, to_string(b.year), b.genre };
        for (int c = 0; c < 5; c++) {
            if (c > 0)
                DrawLine(colX[c], ry, colX[c], ry + ROW_H, { 230, 233, 237, 255 });
            string txt = trunc(fs[c], COL_W[c] - s(8), FONT_SIZE);
            DrawTextEx(customFont, txt.c_str(), { (float)(colX[c] + 4), (float)(ry + (ROW_H - FONT_SIZE) / 2) }, (float)FONT_SIZE, 1, BLACK);
        }
    }

    // Border around list
    DrawRectangleLines((int)listRect.x, (int)listRect.y, (int)listRect.width, (int)listRect.height, { 200, 200, 200, 255 });

    // Scrollbar
    int maxSc = MaxScroll();
    if (maxSc > 0) {
        float sbH = (listRect.height - HEADER_H) * (float)vr / (float)filtered.size();
        float sbY = (listRect.y + HEADER_H) + (listRect.height - HEADER_H - sbH) * (float)scrollOffset / (float)maxSc;
        DrawRectangle((int)(listRect.x + listRect.width - 8), (int)sbY, 8, (int)sbH, { 180, 200, 220, 255 });
    }

    // Status bar
    string st = "Total: " + to_string(books.size()) + " books  |  Showing: " + to_string((int)filtered.size());
    DrawRectangleRec(statusRect, { 0, 103, 192, 255 });
    DrawTextEx(customFont, st.c_str(), { (float)((int)statusRect.x + 6), (float)((int)statusRect.y + 4) }, (float)FONT_SIZE, 1, WHITE);
}

void DrawDialog(bool edit) {
    DrawRectangle(0, 0, W, H, { 0, 0, 0, 60 });

    int dlgW = s(420), dlgH = s(330);
    int dlgX = (W - dlgW) / 2, dlgY = (H - dlgH) / 2;

    DrawRectangle(dlgX, dlgY, dlgW, dlgH, WHITE);
    DrawRectangleLines(dlgX, dlgY, dlgW, dlgH, { 200, 200, 200, 255 });

    string title = edit ? "Edit Book" : "Add Book";
    DrawTextEx(customFont, title.c_str(), { (float)(dlgX + s(20)), (float)(dlgY + s(15)) }, (float)FONT_SIZE, 1, BLACK);

    int fieldStartY = dlgY + s(50);
    int fieldH = s(28);
    int labelW = s(65);
    int inputW = dlgW - s(105);
    int gap = s(38);
    int fs = FONT_SIZE;

    for (int i = 0; i < 5; i++) {
        int fy = fieldStartY + i * gap;
        DrawTextEx(customFont, dlgLabels[i], { (float)(dlgX + s(20)), (float)(fy + (fieldH - fs) / 2) }, (float)fs, 1, { 80, 80, 80, 255 });
        Rectangle ir = { (float)(dlgX + s(20) + labelW), (float)fy, (float)inputW, (float)fieldH };
        DrawInputField(ir, dlgVal[i], i == dlgActive, "");
    }

    int btnY = dlgY + dlgH - s(50);
    int btnW = s(80), btnH = s(30);
    int okX = dlgX + dlgW / 2 - btnW - s(6);
    int cancelX = dlgX + dlgW / 2 + s(6);
    Vector2 mp = GetMousePosition();
    bool okH = CheckCollisionPointRec(mp, { (float)okX, (float)btnY, (float)btnW, (float)btnH });
    bool caH = CheckCollisionPointRec(mp, { (float)cancelX, (float)btnY, (float)btnW, (float)btnH });
    DrawButton({ (float)okX, (float)btnY, (float)btnW, (float)btnH }, "OK", true, okH, CLR_SUCCESS);
    DrawButton({ (float)cancelX, (float)btnY, (float)btnW, (float)btnH }, "Cancel", true, caH, CLR_SECONDARY);
}

void DrawConfirm() {
    DrawRectangle(0, 0, W, H, { 0, 0, 0, 60 });

    int cw = s(360), ch = s(120);
    int cx = (W - cw) / 2, cy = (H - ch) / 2;
    DrawRectangle(cx, cy, cw, ch, WHITE);
    DrawRectangleLines(cx, cy, cw, ch, { 200, 200, 200, 255 });

    DrawTextEx(customFont, confirmMsg.c_str(), { (float)(cx + s(15)), (float)(cy + s(15)) }, (float)FONT_SIZE, 1, BLACK);

    int btnW = s(70), btnH = s(28);
    int btnY = cy + ch - s(45);
    int yesX = cx + cw / 2 - btnW - s(6);
    int noX = cx + cw / 2 + s(6);
    Vector2 mp = GetMousePosition();
    bool yesH = CheckCollisionPointRec(mp, { (float)yesX, (float)btnY, (float)btnW, (float)btnH });
    bool noH = CheckCollisionPointRec(mp, { (float)noX, (float)btnY, (float)btnW, (float)btnH });
    DrawButton({ (float)yesX, (float)btnY, (float)btnW, (float)btnH }, "Yes", true, yesH, CLR_SUCCESS);
    DrawButton({ (float)noX, (float)btnY, (float)btnW, (float)btnH }, "No", true, noH, CLR_SECONDARY);
}

// ---- Actions ----
void StartAdd() {
    for (int i = 0; i < 5; i++) dlgVal[i].clear();
    dlgActive = 0;
    dlgEditIdx = -1;
    state = ADD_DIALOG;
}

void EditBook() {
    if (selFiltered < 0 || selFiltered >= (int)filtered.size()) return;
    int idx = filtered[selFiltered];
    Book b = books.get(idx);
    dlgVal[0] = to_string(b.id);
    dlgVal[1] = b.title;
    dlgVal[2] = b.author;
    dlgVal[3] = to_string(b.year);
    dlgVal[4] = b.genre;
    dlgActive = 0;
    dlgEditIdx = idx;
    state = EDIT_DIALOG;
}

void DeleteBook() {
    if (selFiltered < 0 || selFiltered >= (int)filtered.size()) return;
    confirmIdx = filtered[selFiltered];
    Book b = books.get(confirmIdx);
    confirmMsg = "Delete \"" + b.title + "\"?";
    state = CONFIRM_DELETE;
}

void SubmitBook() {
    int id = atoi(dlgVal[0].c_str());
    if (id > 0) {
        Book nb(id, dlgVal[1], dlgVal[2], atoi(dlgVal[3].c_str()), dlgVal[4]);
        if (state == ADD_DIALOG)
            books.pushBack(nb);
        else
            books.set(dlgEditIdx, nb);
        RebuildFilter();
    }
    state = MAIN;
}

// ---- Input ----
void HandleMainInput() {
    Vector2 mp = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mp, searchRect))
            searchFocused = true;
        else
            searchFocused = false;
    }

    if (searchFocused) {
        HandleTextInput(searchText, 40);
        RebuildFilter();
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
            searchFocused = false;
    }

    int mw = (int)GetMouseWheelMove();
    if (mw != 0 && CheckCollisionPointRec(mp, listRect)) {
        scrollOffset -= mw;
        scrollOffset = max(0, min(scrollOffset, MaxScroll()));
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (!searchFocused) {
            int row = RowFromMouse(mp);
            if (row >= 0) {
                double now = GetTime();
                if (row == lastClickRow && (now - lastClickTime) < 0.4)
                    EditBook();
                lastClickTime = now;
                lastClickRow = row;
                selFiltered = row;
            }
        }
        if (CheckCollisionPointRec(mp, addBtn)) StartAdd();
        else if (CheckCollisionPointRec(mp, editBtn) && selFiltered >= 0) EditBook();
        else if (CheckCollisionPointRec(mp, delBtn) && selFiltered >= 0) DeleteBook();
    }

    if (!searchFocused) {
        if (IsKeyPressed(KEY_A)) StartAdd();
        if (IsKeyPressed(KEY_DELETE) && selFiltered >= 0) DeleteBook();
        if (IsKeyPressed(KEY_ENTER) && selFiltered >= 0) EditBook();
        if (IsKeyPressed(KEY_DOWN)) {
            if (selFiltered < (int)filtered.size() - 1) {
                selFiltered++;
                if (selFiltered >= scrollOffset + VisibleRows())
                    scrollOffset++;
            }
        }
        if (IsKeyPressed(KEY_UP)) {
            if (selFiltered > 0) {
                selFiltered--;
                if (selFiltered < scrollOffset)
                    scrollOffset--;
            }
        }
        if (IsKeyPressed(KEY_EQUAL)) {
            scale = min(scale + 0.25f, 3.0f);
            ApplyScale();
            SetWindowSize(W, H);
            SetupLayout();
            RebuildFilter();
        }
        if (IsKeyPressed(KEY_MINUS)) {
            scale = max(scale - 0.25f, 0.5f);
            ApplyScale();
            SetWindowSize(W, H);
            SetupLayout();
            RebuildFilter();
        }
    }
}

void HandleDialogInput() {
    int dlgW = s(420), dlgH = s(330);
    int dlgX = (W - dlgW) / 2, dlgY = (H - dlgH) / 2;
    int fieldStartY = dlgY + s(50);
    int fieldH = s(28);
    int labelW = s(65);
    int inputW = dlgW - s(105);
    int gap = s(38);

    Vector2 mp = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < 5; i++) {
            int fy = fieldStartY + i * gap;
            Rectangle ir = { (float)(dlgX + s(20) + labelW), (float)fy, (float)inputW, (float)fieldH };
            if (CheckCollisionPointRec(mp, ir)) {
                dlgActive = i;
                return;
            }
        }
    }

    HandleTextInput(dlgVal[dlgActive], 100);

    if (IsKeyPressed(KEY_TAB)) { dlgActive = (dlgActive + 1) % 5; return; }
    if (IsKeyPressed(KEY_ENTER)) { SubmitBook(); return; }
    if (IsKeyPressed(KEY_ESCAPE)) { state = MAIN; return; }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int btnY = dlgY + dlgH - s(50);
        int btnW = s(80), btnH = s(30);
        int okX = dlgX + dlgW / 2 - btnW - s(6);
        int cancelX = dlgX + dlgW / 2 + s(6);
        if (CheckCollisionPointRec(mp, { (float)okX, (float)btnY, (float)btnW, (float)btnH }))
            SubmitBook();
        else if (CheckCollisionPointRec(mp, { (float)cancelX, (float)btnY, (float)btnW, (float)btnH }))
            state = MAIN;
    }
}

void HandleConfirmInput() {
    int cw = s(360), ch = s(120);
    int cx = (W - cw) / 2, cy = (H - ch) / 2;
    int btnW = s(70), btnH = s(28);
    int btnY = cy + ch - s(45);
    int yesX = cx + cw / 2 - btnW - s(6);
    int noX = cx + cw / 2 + s(6);

    Vector2 mp = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mp, { (float)yesX, (float)btnY, (float)btnW, (float)btnH })) {
            books.removeAt(confirmIdx);
            RebuildFilter();
            state = MAIN;
            return;
        }
        if (CheckCollisionPointRec(mp, { (float)noX, (float)btnY, (float)btnW, (float)btnH })) {
            state = MAIN;
            return;
        }
    }

    if (IsKeyPressed(KEY_Y)) {
        books.removeAt(confirmIdx);
        RebuildFilter();
        state = MAIN;
        return;
    }
    if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE)) {
        state = MAIN;
        return;
    }
}

// ---- Entry point ----
int main() {
    loadFromFile(books);

    ApplyScale();
    InitWindow(W, H, "Book Library");
    SetTargetFPS(60);
    customFont = LoadFontEx("C:/Windows/Fonts/consola.ttf", 64, 0, 0);
    SetupLayout();
    RebuildFilter();

    while (!WindowShouldClose()) {
        switch (state) {
            case MAIN:               HandleMainInput();   break;
            case ADD_DIALOG:
            case EDIT_DIALOG:        HandleDialogInput(); break;
            case CONFIRM_DELETE:     HandleConfirmInput(); break;
        }

        BeginDrawing();
        DrawMain();
        switch (state) {
            case ADD_DIALOG:   DrawDialog(false); break;
            case EDIT_DIALOG:  DrawDialog(true);  break;
            case CONFIRM_DELETE: DrawConfirm();   break;
            default: break;
        }
        EndDrawing();
    }

    UnloadFont(customFont);
    saveToFile(books);
    CloseWindow();
    return 0;
}
