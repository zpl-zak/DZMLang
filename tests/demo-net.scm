(define server (make-socket))
(socket-listen server 1 7777) ;; sock, maxconn, port

;; accept 1 client
(define client (socket-accept server))

;; send him a string
(socket-write-string client 12 "Hello World!")

;; receive a string from him
(define response (trim-string
                  (car (socket-read-string client 12))))

;; print the response
(write response)

;; close sockets
(socket-close client)
(socket-close server)

;; demo used with: "ncat -e /bin/cat 127.0.0.1 7777" ..
