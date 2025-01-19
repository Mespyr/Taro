;;; -*- lexical-binding: t -*-

(defconst taro-mode-syntax-table
  (with-syntax-table (copy-syntax-table)
    ;; Chars are the same as strings
    (modify-syntax-entry ?' "\"")
    (syntax-table)))

(defconst taro-builtin-value
  '("true" "false" "none"))

(defconst taro-keywords
  '("if" "else" "while" "use" "fn"
    "type" "expr" "extern" "as"))

(defconst taro-builtin '("call" "unwrap"))

(defconst taro-types
  '("i64" "i32" "u64" "u32" "u8" "f64" "f32" "bool" "string" "list"))

(defconst taro-highlights `(
  ("#.*" . font-lock-comment-face)
  ("-?\\<-?[0-9]+\\(\\.[0-9]+\\)?\\>"        . font-lock-constant-face)
  (,(regexp-opt taro-builtin-value 'symbols) . font-lock-constant-face)
  (,(regexp-opt taro-keywords 'symbols)      . font-lock-keyword-face)
  (,(regexp-opt taro-builtin 'symbols)       . font-lock-builtin-face)
  (,(regexp-opt taro-types 'symbols)         . font-lock-type-face)))

(define-derived-mode taro-mode prog-mode "Taro"
  "Major Mode for editing Taro source code."
  :syntax-table taro-mode-syntax-table
  (setq font-lock-defaults '(taro-highlights)))

(add-to-list 'auto-mode-alist '("\\.taro\\'" . taro-mode))
