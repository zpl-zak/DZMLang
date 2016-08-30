;;; Pipe chat, or better said, echo messenger just returns what you say, but also handles input via pipe

(define (get)
  (begin
    (trim-string (read-string))
    (get)))

(define (put)
  (begin
    (write (read-string))
    (put)))

(spawn '(get) '(put))
