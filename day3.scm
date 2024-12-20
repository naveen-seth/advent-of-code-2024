(use-modules (ice-9 match)
	     (ice-9 regex)
	     (ice-9 textual-ports))

;; slurp input
(define input-file "input.dat")
(define input (call-with-input-file input-file get-string-all))

;; part 1
(define mult-cmd-rx (make-regexp "mul\\(([1-9][0-9]*),([1-9][0-9]*)\\)"))

(define (run-program input)
  (fold-matches mult-cmd-rx input 0
		(lambda (m acc)
		  (let ((a (string->number (match:substring m 1)))
			(b (string->number (match:substring m 2))))
		    (+ acc (* a b))))))

;; part 2
(define do-cmd-rx (make-regexp "do\\(\\)"))
(define dont-cmd-rx (make-regexp "don't\\(\\)"))

(define (run-segmented-program input)
  (let loop ((pos 0)
	     (acc 0))
    (match (regexp-exec dont-cmd-rx input pos)
      (#f       ; case: no disable ahead
       (+ acc (run-program (substring input pos))))
      (dont-cmd ; case: disable ahead
       (let* ((segment (substring input pos (match:start dont-cmd)))
	      (acc (+ acc (run-program segment))))
	 (match (regexp-exec do-cmd-rx input (match:end dont-cmd))
	   (#f     ; case: disabled until end of input
	    acc)
	   (do-cmd ; case: next enabled segment
	    (loop (match:end do-cmd) acc))))))))

(display "Part 1: ")
(display (run-program input))
(newline)
(display "Part 2: ")
(display (run-segmented-program input))
(newline)
