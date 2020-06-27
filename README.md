# Bs-Cheerio
(Cheerio)[https://www.npmjs.com/package/cheerio] bindings for ReasonML.

# Quick start
## Installation
> npm i bs-cheerio
## Examples
1. Selecting the `href` of a link.
   ```reason
   open BsCheerio;
   open Js.Promise;
   open PromiseMonad;

   let releases_url = "https://github.com/VSCodium/vscodium/releases";

   Fetch.fetch(releases_url)
   >>= Fetch.Response.text
   >>- (
     html => {
       Cheerio.load(html)
       ->Cheerio.select(
           {js|.d-flex.flex-items-center.min-width-0[href$="amd64.deb"]|js},
         )
       ->Element.attr1("href");
     }
   )
   >>- Js.Console.log;
   // => "/VSCodium/vscodium/releases/download/1.46.1/codium_1.46.1-1592564058_amd64.deb"
   ```
1. Extracting texts from matching elements
  ```reason
  dom
    ->Cheerio.select(".pochodzenie > tbody > tr .pochodzenie_jezyk")
    ->Element.map((_, e) => e->Element.load->Element.text0)
    ->Element.toArray
    // => array(string)
  ```