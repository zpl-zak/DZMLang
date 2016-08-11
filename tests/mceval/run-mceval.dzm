(define true #t)
(define false #f)
(define display write)
(define newline (lambda () (write-char #\newline)))

(load "tests/mceval/mceval.scm")

(define the-global-environment (setup-environment))
(driver-loop)