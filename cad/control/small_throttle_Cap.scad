$fn=360;
screw_hole_dia=2.8;

difference()
{

sphere(d=50);
//hollow sphere
sphere(d=45);
//flat back side
translate([-26,0,-26])
cube(52);

/*
removed for looks
union()
{
//top
translate([0,0,21])
rotate([90,0,0])
screw_set();
//left  
translate([21,0,0])
rotate([90,0,0])
screw_set();
//right
translate([-21,0,0])
rotate([90,0,0])
screw_set();
//bottom 
translate([0,0,-21])
rotate([90,0,0])
screw_set();
}
*/


}

module screw_set()
{
cylinder(h=53,d=5);

}
module screw_set2()
{
difference()
{
cylinder(h=13,d=5);
translate([0,0,-1])
cylinder(h=17,d=screw_hole_dia);
}



}


intersection()
{
union()
{
//top
translate([0,0,21])
rotate([90,0,0])
screw_set2();
//left  
translate([21,0,0])
rotate([90,0,0])
screw_set2();
//right
translate([-21,0,0])
rotate([90,0,0])
screw_set2();
//bottom 
translate([0,0,-21])
rotate([90,0,0])
screw_set2();
}
sphere(d=50);
}

