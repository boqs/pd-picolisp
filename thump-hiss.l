(setq ticks 0)

(pd-post "badger")

(de bang ()
    (setq ticks (% (+ ticks 1) 16))
    (scheduler ticks))

(de scheduler (ticks)
    (when (= (% ticks 3) 0)
      (pd-message 'bd))
    (and (pd-message 'hh)))
