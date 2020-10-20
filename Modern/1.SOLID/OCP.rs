#![allow(dead_code)]
trait Bindable<T> {
    fn run(&self, &T);
}
struct Binded<'a, T> {
    a : Box<dyn Bindable<T> + 'a>,
    b : Box<dyn Bindable<T> + 'a>,
}
impl<'a, B: 'a> Binded<'a, B> {
    fn new<T, U>(a: T, b: U) -> Self 
    where T: Bindable<B> + 'a, U: Bindable<B> + 'a
    {
        Binded {
            a: Box::new(a),
            b: Box::new(b),
        }
    }
}
impl<'a, T> Bindable<T> for Binded<'a, T> {
    fn run(&self, val: &T) {
        self.a.run(val);
        self.b.run(val);
    }
}

trait BindOps<'a, T : 'a> {
    fn bind<U>(self, other: U) -> Binded<'a, T>
    where U: Bindable<T> + 'a, Self: Sized + Bindable<T> + 'a
    {
        Binded::new(self, other)
    }
}
impl<'a, U: 'a, T: Bindable<U>> BindOps<'a, U> for T {
}

struct Plus1();
struct Plus2();
struct Plus3();

impl Bindable<i32> for Plus1 {
    fn run(&self, val: &i32) {
        println!("{}", val + 1);
    }
}
impl Bindable<i32> for Plus2 {
    fn run(&self, val: &i32) {
        println!("{}", val + 2);
    }
}
impl Bindable<i32> for Plus3 {
    fn run(&self, val: &i32) {
        println!("{}", val + 3);
    }
}

fn main() {
    let binded = Plus1().bind(Plus2()).bind(Plus3().bind(Plus1()));
    binded.run(&1);
}