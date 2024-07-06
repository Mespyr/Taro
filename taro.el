(defconst taro-mode-syntax-table
  (with-syntax-table (copy-syntax-table)
    ;; Chars are the same as strings
    (modify-syntax-entry ?' "\"")
    (syntax-table)))

(defconst taro-builtin-value
  '("true" "false" "none"))

(defconst taro-keywords
  '("if" "else" "while" "import" "fn"
    "type" "expr" "extern" "as"))

(defconst taro-builtin '("call"))

(defconst taro-types
  '("I64" "I32" "U64" "U32" "U8" "F64" "F32" "Bool" "Any"))

(defconst taro-highlights `(
  ("#.*" . font-lock-comment-face)
  ("-?\\<-?[0-9]+\\(\\.[0-9]+\\)?\\>"         . font-lock-constant-face)
  (,(regexp-opt taro-builtin-value 'symbols) . font-lock-constant-face)
  (,(regexp-opt taro-keywords 'symbols)      . font-lock-keyword-face)
  (,(regexp-opt taro-builtin 'symbols)       . font-lock-builtin-face)
  (,(regexp-opt taro-types 'symbols)         . font-lock-type-face)))

(define-derived-mode taro-mode prog-mode "Taro"
  "Major Mode for editing Taro source code."
  :syntax-table taro-mode-syntax-table
  (setq font-lock-defaults '(taro-highlights)))

(add-to-list 'auto-mode-alist '("\\.taro\\'" . taro-mode))
