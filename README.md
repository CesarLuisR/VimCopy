# VimoText (vimc)

A simple, terminal-based text editor inspired by Vim.

---

## 🚀 How to Run

There are **two** ways to get started. In either case, you need **g++** installed on your machine.

1. **Using the installer**  
   - Run the `install.bat` script (located one level above `VimoText\`) from a Windows CMD:  
     ```bat
     install.bat
     ```  
   - Close and reopen your terminal.  
   - You can now launch the editor from **any** folder by typing:  
     ```
     vimc
     ```

2. **Manual compilation**  
   - Open a terminal (or CMD) and navigate into the `VimoText` directory.  
   - Compile all `.cpp` files into an executable:  
     ```bash
     g++ *.cpp -o vimc.exe
     ```  
   - Run it directly from that folder:  
     ```bash
     ./vimc.exe
     ```

---

## 📖 What Is It & What It Does

**VimoText** (`vimc`) is a lightweight, command-mode text editor for the Windows console, heavily inspired by Vim. It supports:

- **Normal mode**, for navigation and commands  
- **Insert mode**, for typing new text  
- **Visual mode**, for selecting blocks of text  

It keeps an undo/redo history and lets you save and load files, all without leaving your terminal.

---

## ⌨️ Supported Commands

| Command | Description                                       |
|---------|---------------------------------------------------|
| `h`     | Move cursor **left**                              |
| `j`     | Move cursor **down**                              |
| `k`     | Move cursor **up**                                |
| `l`     | Move cursor **right**                             |
| `gg`    | Go to **first** line                              |
| `G`     | Go to **last** line                               |
| `u`     | **Undo** last change                              |
| `U`     | **Redo** last undone change                       |
| `i`     | Enter **Insert** mode                             |
| `Esc`   | Return to **Normal** mode                         |
| `:q`    | **Quit** editor (if no unsaved changes)           |
| `:w`    | **Save** current file                             |
| `w`     | Move to **next** word                             |
| `b`     | Move to **previous** word                         |
| `dd`    | **Delete** current line                           |
| `dw`    | **Delete** current word                           |
| `v`     | Enter **Visual** mode (start selection)           |
| `y`     | **Yank** (copy) selected text                     |
| `p`     | **Paste** yanked text after cursor                |
| `a`     | **Append** after cursor (enter Insert mode)       |
| `A`     | **Append** at end of line (enter Insert mode)     |
| `o`     | **Open** new line **below** (enter Insert mode)   |
| `O`     | **Open** new line **above** (enter Insert mode)   |

---

## 🎯 Takeaways & Lessons Learned

During this project I **did not** initially apply several best practices, and by missing them I learned their true importance. Going forward, I will always incorporate:

- **Clean Architecture (MVC):**  
  I skipped designing a clear separation of Model, View, and Controller at the start, which made the code harder to maintain. Now I understand how crucial architectures like MVC are for the scalability of large projects.
  
- **Design & Documentation First:**  
  I jumped straight into coding without a high-level system design or documentation. I learned that planning and writing down architecture decisions up front saves time and avoids confusion later.

- **Design Patterns:**  
  I implemented a basic Command pattern for undo/redo, but omitted other useful patterns (Observer, Factory, etc.). Not using them taught me their value for organizing complex logic.

- **Automated Builds & Installs:**  
  I relied on manual scripts rather than a proper build system. Failing to use tools like CMake or containers showed me the importance of automating compilation and installation across different environments.

By realizing what I missed, these lessons will now be integral to every future project.

Enjoy editing! 🚀  
