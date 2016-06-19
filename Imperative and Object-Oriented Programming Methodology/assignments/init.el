(menu-bar-mode -1)
(tool-bar-mode -1)
(scroll-bar-mode -1)
(global-linum-mode t)

(setq-default indent-tabs-mode nil)
;(setq-default tab-width 4)
;(setq indent-line-function 'insert-tab)

(require 'package) ;; You might already have this line
(add-to-list 'package-archives
             '("melpa" . "https://melpa.org/packages/"))
(when (< emacs-major-version 24)
  ;; For important compatibility libraries like cl-lib
  (add-to-list 'package-archives '("gnu" . "http://elpa.gnu.org/packages/")))
(package-initialize) ;; You might already have this line
(require 'yasnippet)
(yas-global-mode 1)
;;; Auto complete mod 
;;; should be loaded after yasnippet so that they can work together 
(require 'auto-complete-config) 
(add-to-list 'ac-dictionary-directories "~/.emacs.d/ac-dict") 
(ac-config-default) 
;;; set the trigger key so that it can work together with yasnippet on tab key, 
;;; if the word exists in yasnippet, pressing tab will cause yasnippet to 
;;; activate, otherwise, auto-complete will 
(ac-set-trigger-key "TAB") 
(ac-set-trigger-key "<tab>")
(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(inhibit-startup-screen t))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )
