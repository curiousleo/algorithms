(*
 * protoveb.ml
 *
 * Incomplete Proto-vEB implementation featuring "init", "member",
 * "insert", "delete", and "dotprint" (for debugging)
 *
 * Leonhard Markert, 2012
 * Licensed under Creative Commons Attribution (cc-by).
 *)

(*
 * A Proto-vEB is either
 *   - a tuple (u : int, summary : protoveb ref,
 *              cluster : protoveb ref list), encoded in "Container", or
 *   - a tuple (A0 : bool, A1 : bool), encoded in "Base".
 *)
type protoveb =
   Container of int * protoveb ref * protoveb ref list
 | Base of bool * bool

(* high, low and index are helper functions. *)
let high  u x   = truncate ((float x) /. (sqrt (float u)))
let low   u x   = x mod (truncate (sqrt (float u)))
let index u x y = x * (truncate (sqrt (float u))) + y

(* Create an empty Proto-vEB of size u, where u = 2^2^k for some
 * non-negative integer k. *)
let rec init = function
   2 -> ref (Base (false, false))
 | u ->
     let sqrtu = truncate (sqrt (float u)) in
     let s = init sqrtu in
     let cref = ref [] in
     for i = 1 to sqrtu do
       cref := init sqrtu :: !cref
     done ;
     ref (Container (u, s, !cref))

(* Check if x is a member of the Proto-vEB v. *)
let rec member v x =
  match !v with
     (Base (p, q))         -> if x = 0 then p else q
   | (Container (u, s, c)) ->
       let hi = high u x in
       let lo = low u x in
       member (List.nth c hi) lo

(* Insert x into the Proto-vEB v. *)
let rec insert v x =
  match !v with
     (Base (p, q))         ->
       if x = 0 then v := Base (true, q) else v := Base (p, true)
   | (Container (u, s, c)) ->
       let hi = high u x in
       let lo = low u x in
       insert (List.nth c hi) lo ;
       insert s hi

(*
 * Delete x from the Proto-vEB v, and return a bool indicating whether
 * there are any values left in the Proto-vEB v.
 *
 * In CLRS-style pseudocode:
 *
 * Proto-vEB-Delete(V,x)
 *     if V.u == 2
 *         V.A[x] = 0
 *         return V.A[(x+1) mod 2]
 *     else
 *         if Proto-vEB-Delete(V.cluster[high(x)],low(x))
 *             return true
 *         else
 *             return Proto-vEB-Delete(V.summary,high(x))
 *)
let rec delete v x =
  match !v with
     (Base (p, q))           ->
       if x = 0 then (v := Base (false, q) ; q)
                else (v := Base (p, false) ; p)
   | (Container (u, s, c)) ->
       let hi = high u x in
       let lo = low u x in
       let other = delete (List.nth c hi) lo in
       if not other then delete s hi else true

(* Print a DOT representation of the Proto-vEB v to out. *)
let dotprint v out =
  let rec dot w parent self =
    if self <> "root" then Printf.fprintf out "\t%s -> %s;\n" parent self ;
    match !w with
       (Base (p, q))         ->
         let pi = if p then 1 else 0 in
         let qi = if q then 1 else 0 in
         Printf.fprintf out "\t%s [label=\"{%d|%d}\"];\n" self pi qi
     | (Container (u, s, c)) ->
         Printf.fprintf out "\t%s [label=\"%d|<s>s" self u ;
         let iref = ref 0 in
         List.iter
           (function z ->
             Printf.fprintf out "|<%d>%d" !iref !iref ;
             iref := !iref + 1)
           c ;
         Printf.fprintf out "\"];\n" ;
         Printf.fprintf out "\t%ss [style=filled, fillcolor=grey]\n" self ;
         dot s (self ^ ":s") (self ^ "s") ;
         let jref = ref 0 in
         List.iter
           (function z ->
             let j = string_of_int !jref in
             dot z (self ^ ":" ^ j) (self ^ j) ;
             jref := !jref + 1)
           c in
  Printf.fprintf out "digraph cdll {\n\tnode [shape=Mrecord];\n" ;
  dot v "root" "root" ;
  Printf.fprintf out "}\n"

let () = if !Sys.interactive then () else
  (* Create empty Proto-vEB of size 16. *)
  let pref = init 16 in
  (* Insert 2, 3, 4, 5, 7, 14, 15. *)
  List.iter (insert pref) [2; 3; 4; 5; 7; 14; 15] ;
  (* Print to "protoveb1.dot". *)
  dotprint pref (open_out "protoveb1.dot") ;
  (* Delete 14. *)
  ignore (delete pref 14) ;
  (* Print to "protoveb1.dot". *)
  dotprint pref (open_out "protoveb2.dot") ;
  (* Delete 15. *)
  ignore (delete pref 15) ;
  (* Print to "protoveb1.dot". *)
  dotprint pref (open_out "protoveb3.dot")
