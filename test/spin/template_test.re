open Test_framework;
open Spin;

describe("Test Template", ({test, describe, _}) => {
  test("generate", ({expect, _}) => {
    let dest = Test_utils.get_tempdir("generate");
    let source =
      Source.LocalDir(
        Utils.Filename.join(["test", "resources", "sample_template"]),
      );
    Template.generate(source, dest, ~useDefaults=true);

    let generatedFiles = Utils.Sys.ls_dir(dest);
    let generatedFile =
      Utils.Filename.join([dest, "dirname", "filename.txt"]);
    let generatedConf = Utils.Filename.join([dest, ".spin"]);

    expect.list([generatedConf, generatedFile]).toEqual(generatedFiles);

    let generatedContent = Stdio.In_channel.read_all(generatedFile);
    expect.equal("Hello World!", generatedContent);
  });

  test("generate configuration", ({expect, _}) => {
    let dest = Test_utils.get_tempdir("generate_configuration");
    let source =
      Source.LocalDir(
        Utils.Filename.join(["test", "resources", "sample_template"]),
      );

    Template.generate(source, dest, ~useDefaults=true);

    let generatedConfContent =
      Sexplib.Sexp.load_sexps(Utils.Filename.join([dest, ".spin"]));

    let expected =
      expect.list([
        Sexp.List([
          Sexp.Atom("Source"),
          Sexp.Atom(
            Utils.Filename.join(["test", "resources", "sample_template"]),
          ),
        ]),
        Sexp.List([
          Sexp.Atom("Cfg_str"),
          Sexp.Atom("dirname"),
          Sexp.Atom("dirname"),
        ]),
        Sexp.List([
          Sexp.Atom("Cfg_str"),
          Sexp.Atom("filename"),
          Sexp.Atom("filename"),
        ]),
        Sexp.List([
          Sexp.Atom("Cfg_str"),
          Sexp.Atom("content"),
          Sexp.Atom("Hello World!"),
        ]),
      ]);

    expected.toEqual(generatedConfContent);
  });

  test("ignore files", ({expect, _}) => {
    let dest = Test_utils.get_tempdir("ignore_files");
    let source =
      Source.LocalDir(
        Utils.Filename.join(["test", "resources", "template_with_ignores"]),
      );
    Template.generate(source, dest, ~useDefaults=true);

    let generatedFiles =
      Utils.Sys.ls_dir(dest) |> List.sort(~compare=String.compare);
    let expected =
      expect.list(
        [
          Utils.Filename.concat(dest, ".spin"),
          Utils.Filename.concat(
            dest,
            "this_one_matches_but_condition_is_false",
          ),
          Utils.Filename.concat(dest, "f.dont_ignore_me"),
        ]
        |> List.sort(~compare=String.compare),
      );
    expected.toEqual(generatedFiles);
  });
});