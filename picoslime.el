(defvar *picoslime* nil)
(defvar *picoslime-host* "localhost")
(defvar *picoslime-port* 6000)

(defun picoslime-bang-pd (str)
  (unless (and *picoslime*
	       (process-live-p *picoslime*))
    (setq *picoslime* (open-network-stream "picoslime-banger"
					   "*picoslime-connection*"
					   *picoslime-host*
					   *picoslime-port*)))
  (network-stream-command *picoslime* (concat str ";") nil))

(defun picoslime-grab-current-expr ()
  (push-mark)
  (let ((end (point)))
    (backward-sexp)
    (prog1 (setq *foo* (buffer-substring-no-properties (point) end))
      (forward-sexp)
      (pop-mark))))

(defun pd-pil-bang-at-point ()
  (interactive)
  (picoslime-bang-pd (picoslime-grab-current-expr)))
