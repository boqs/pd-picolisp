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
	     n))

(de schedule-tick ()
    (set 'schedule-queue
	 (let (schedule-queue-old schedule-queue)
	   (let (schedule-queue Nil)
	     (let (schedule-queue-new
		   (mapcan
		    '((x) (if (<= (dec (car x)) 0)
			      (prog ((cdr x))
			       Nil)
			      (list (cons (dec (car x))
					  (cdr x)))))
		     schedule-queue-old ) )
	       (append schedule-queue schedule-queue-new) ) ) ) ) )

(de bang ()
    (schedule-tick))

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
    (lambda)
    (for x (- n 1) (schedule lambda (* x speed))))

##(flam 100 10 '(() (pd-message 'hh)))


(de pat1 ()
    (flam 10 7 '(() (pd-message 'bd))))

(de pat2 ()
    (flam 4 4 '(() (pd-message 'hh))))

## (pat1)
## (pat2)

## (schedule-loop pat1 70)
## (schedule-loop pat2 16)
## (schedule-loop '(() (pd-message 'hh)) 4)

## (reset-schedule-queue)
(de hush ()
    (reset-schedule-queue))
