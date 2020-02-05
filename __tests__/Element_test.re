open Jest;
open Expect;

let html = {|
  <!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <meta http-equiv="X-UA-Compatible" content="ie=edge"/>
  <title>Document</title>
</head>
<body>
  <ul className="ul_class">
    <li class="ul_class__li_class" id="LI_0_id">LI_0</li>
    <li class="ul_class__li_class" id="LI_1_id" data-d="LI_1_data">LI_1</li>
    <li class="ul_class__li_class">LI_2</li>
    <li class="ul_class__li_class"></li>
  </ul>
</body>
</html>
|};

describe("text", () => {
  let dom = Cheerio.load(html);
  test("with text", () => {
    dom->Cheerio.select("ul li#LI_0_id")->Element.text->expect
    |> toBe("LI_0")
  });

  test("without text", () => {
    dom->Cheerio.select("ul li:nth-child(4)")->Element.text
    |> expect
    |> toEqual("")
  });
});

describe("html", () => {
  let dom = Cheerio.load(html);
  test("works", () => {
    dom->Cheerio.select("ul")->Element.html
    |> expect
    |> toContainString({|<li class="ul_class__li_class">LI_2</li>|})
  });
});

describe("attr1", () => {
  let dom = Cheerio.load(html);
  test("Some(p) when a prop exists", () => {
    dom
    ->Cheerio.select("li#LI_1_id")
    ->Element.attr1("data-d")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(Some("LI_1_data"))
  });
  test("None when a prop doesn't exist", () => {
    dom
    ->Cheerio.select("li#LI_1_id")
    ->Element.attr1("made-up-prop")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(None)
  });
  test("None when an element doesn't exist", () => {
    dom
    ->Cheerio.select("li#IDontExist")
    ->Element.attr1("me-neither")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(None)
  });
});

describe("attr2", () => {
  let dom = Cheerio.load(html);
  test("Sets value of an existing element", () => {
    dom
    ->Cheerio.select("li#LI_1_id")
    ->Element.attr2("data-sth", "some-value")
    ->Element.attr1("data-sth")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(Some("some-value"))
  });
  test("None when trying to set a value of a nonexisting element", () => {
    dom
    ->Cheerio.select("li#IDontExist")
    ->Element.attr2("data-sth", "some-value")
    ->Element.attr1("data-sth")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(None)
  });
  test("Removes the attr if `null` is passed", () => {
    dom
    ->Cheerio.select("li#LI_1_id")
    ->Element.attr2("data-d", [%bs.raw "null"])
    ->Element.attr1("data-d")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(None)
  });
});

describe("next", () => {
  let dom = Cheerio.load(html);
  test("Some when found the next node", () => {
    dom
    ->Cheerio.select("li#LI_0_id")
    ->Element.next0
    ->Element.attr1("id")
    ->Js.Nullable.toOption
    |> expect
    |> toEqual(Some("LI_1_id"))
  });
});