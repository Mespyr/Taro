(defconst ochre-mode-syntax-table
  (with-syntax-table (copy-syntax-table)
    ;; Chars are the same as strings
    (modify-syntax-entry ?' "\"")
    (syntax-table)))

(defconst ochre-keywords
  '("if" "else" "while" "import" "fn"
    "const" "type" "expr" "extern" "as"))

(defconst ochre-builtin
  '("call" "cast" "new"))

(defconst ochre-types
  '("I64" "I32" "U64" "U32" "U8" "F64" "F32" "Bool" "Any"))

(defconst ochre-highlights `(
  ("#.*" . font-lock-comment-face)
  ("-?\\<-?[0-9]+\\(\\.[0-9]+\\)?\\>"    . font-lock-constant-face)
  (,(regexp-opt ochre-keywords 'symbols) . font-lock-keyword-face)
  (,(regexp-opt ochre-builtin 'symbols)  . font-lock-builtin-face)
  (,(regexp-opt ochre-types 'symbols)    . font-lock-type-face)))

(define-derived-mode ochre-mode prog-mode "Ochre"
  "Major Mode for editing Ochre source code."
  :syntax-table ochre-mode-syntax-table
  (setq font-lock-defaults '(ochre-highlights)))

(add-to-list 'auto-mode-alist '("\\.och\\'" . ochre-mode))
