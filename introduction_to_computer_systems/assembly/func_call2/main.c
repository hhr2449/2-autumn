void func1(a,b,c)
int a,b,c;
{}
void func2(a,b,c,d,e,f)
int a,b,c,d,e,f;
{
    func1(a, b, c);
}
int main()
{
  int a,b,c,d,e,f,g,h;
  a = 1;
  b = 2;
  c = 3;
  d = 4;
  e = 5;
  f = 6;
  g = 7;
  h = 8;

  func1(a,b,c);
  func2(a,b,c,d,e,f);
}