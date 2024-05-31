use rocket::{get, launch, routes};
use rocket::fs::FileServer;
use rocket::response::content::RawHtml;

#[get("/")]
fn index() -> RawHtml<String> {
    RawHtml(std::fs::read_to_string("www/index.html").unwrap())
}

#[launch]
fn rocket() -> _ {
    rocket::build()
        .mount("/", routes![index])
        .mount("/", FileServer::from("www"))
}