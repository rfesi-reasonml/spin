module type ConfigFile = {
  type t;
  type doc;
  type cst;

  let path: string;

  let cst_of_sexp: Sexp.t => cst;
  let t_of_cst:
    (
      ~useDefaults: bool,
      ~models: list((string, Jingoo.Jg_types.tvalue)),
      list(cst)
    ) =>
    t;
  let doc_of_cst: list(cst) => doc;
};

module Make = (C: ConfigFile) => {
  open Sexplib;

  type t = C.t;
  type doc = C.doc;

  let path = dirname => Utils.Filename.concat(dirname, C.path);

  let parse = (~useDefaults=false, ~models=[], dirname: string): t => {
    path(dirname)
    |> Sexp.load_sexps
    |> List.map(~f=C.cst_of_sexp)
    |> C.t_of_cst(~useDefaults, ~models);
  };

  let parse_doc = (dirname: string): doc => {
    path(dirname)
    |> Sexp.load_sexps
    |> List.map(~f=C.cst_of_sexp)
    |> C.doc_of_cst;
  };
};

module Doc = Make(ConfigFile__Doc);
module Generators = Make(ConfigFile__Generators);
module Template = Make(ConfigFile__Template);
module Project = Make(ConfigFile__Project);
