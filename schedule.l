(de reset-schedule-queue ()
    (set 'schedule-queue Nil) )

(de nest (fn ticks)
   (if (<= ticks 0)
      fn
      (list Nil
	 (cons 'quote
	    (nest fn (- ticks 1)) ) ) ) )

(de schedule (lambda ticks)
   (push 'schedule-queue
      (cons ticks
	 lambda ) ) )

(de schedule-loop (lambda n)
   (schedule (list Nil
		(list (cons 'quote lambda))
	       (list 'schedule-loop (cons 'quote lambda) n))
      (- n  1)) )

(de tick ()
    (set 'schedule-queue
	 (let (schedule-queue-old schedule-queue)
	   (let (schedule-queue Nil)
	     (let (schedule-queue-new
		   (mapcan
		    '((x) (if (<= (car x) 0)
			      (prog ((cdr x))
			       Nil)
			      (list (cons (dec (car x))
					  (cdr x)))))
		     schedule-queue-old ) )
	       (append schedule-queue schedule-queue-new) ) ) ) ) )

(de schedule-demo ()
   (reset-schedule-queue)
   (schedule '(() (prinl 'monkey)) 3)
   (schedule '(()
	       (prinl 'badger)
	       (schedule '(() (prinl 'rat))
		  3 ))
      2 )
   schedule-queue )

(de flam (n speed lambda)
    (for x n (schedule lambda (* x speed))))

##(flam 100 10 '(() (pd-message 'hh)))


(de pat1 ()
    (flam 10 7 '(() (pd-message 'bd))))

## (pat1)
## (schedule-loop pat1 79)

(de pat2 ()
    (schedule '(() (pd-message 'bd)) 5)
    (flam 20 2 '(() (pd-message 'hh))))

## (schedule-loop pat2 45)

## (reset-schedule-queue)
