$fn=360;


main_dia=7.9;
main_height=150;

difference()
{
union()
{
cylinder(h=main_height,d=main_dia);
translate([0,0,main_height])
cylinder(h=2.5,d1=main_dia,d2=main_dia-2);

cylinder(h=2.5,d=10);
}
translate([0,0,-1])
cylinder(h=main_height+5,d=2.8);
}