# Professional Development: ReasonML for fullstack web development
# Goals

* Exploring a modern, functional language which can be written on both web servers (via OCaml native compile) and clients (via Bucklescript traspilation to Javascript)
in order to facilitate sharing of the maximum amount of code, particularly data structure definitions and validation logic.

* Utilizing stronger types on the frontend and exploring a typed Javascript (even more so than Typescript or Flow)

* Poking around with how functional languages would and could do server definition (See [this paper](https://monkey.org/~marius/funsrv.pdf) for more information here.)

# Findings

1) It works! It works well in fact! :)
2) ReasonML's compiler (well, bucklescript/ocaml's compiler) is terse and does not well define where errors happen.
  You can start to get a sense where the problem lies after a while, but often you're half-guessing.
  This is also partially a problem of learning the language while doing more advanced work witht he language, as I was guessing at some syntactical constructs and the tooling doesn't handle that guessing well.
3) Sharing the datastructure definition prevents a series of potentially very nasty bugs from arising due to incompatibilities with the datastructure defintions on either side of the wire, which I have come to really appreciate.
4) Pattern matching (and other ML-isms, such as algebraic typing) allow a lot of checking to move to build time rather than run time.
  Further, those checks to be completely exhaustive rather than speculatively so.
5) If you want to interact with something like numpy etc, you might need to handle things in a more round about way (or just implement in python), but I believe that the weight we place on that is partially just historical bias on our part (but that does not make it less real).
   There more than likely exist similar libraries for Reason/OCaml and you can use those, but the cost of that must be factored in.

# Conclusions

Overall, I suppose that the benefits of it server side are similar to writing in Rust or something akin to Rust, and the benefits of writing it on the client are probably much greater than that (no more `undefined`!).
However you take a cut in development speed for those benefits.

I believe it is likely worth it for frontend code.
Backend code may not see the same benefits outside of a vacuum.
I do think the language lends itself well to writing server code, but more investigation will need to happen before I can speak more thoroughly on that matter.
In particular, I am unfamiliar with the breadth of libraries available for OCaml/ReasonML and that ecosystem typically has a large effect on implementation difficulty and therefore suitability.

Writing it on both the client and server yields even greater benefits than one or the other.
Teams can share accrued knowledge of the language. This is a similar benefit to someone writing express.js on the server and js on the frontend, but without all of the pitfalls of Javascript development.
Teams also share the knowledge of the data structures as well as type hierarchy and deeper code organization, leading to decreased spin up time after the team learns the language.
Potentially, developers could also develop code that works on both native and JS platforms.
This would require some careful consideration of the libraries in use in either code base and may be rather complex.
This work of integrating the standard libraries of both JS and OCaml together is happening in mainline Reason development at the time of this writing(June 2020) and the library in question (Tablecloth) has promise!

