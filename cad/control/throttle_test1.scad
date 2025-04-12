$fn=360;

screw_hole_dia=2.8;




difference()
{
//test body
union()
{
rotate([90,0,0])
cylinder(h=25,d1=75,d2=50);

rotate([90,0,0])
cylinder(h=125,d=50);

//slide mount 1
hull()
{
translate([0,-17.5,0])
rotate([90,0,0])
cylinder(h=50,d=50);

translate([-10,-52.5,27])
cube([20,20,5]);
}

translate([0,-60,0])
hull()
{
translate([0,-17.5,0])
rotate([90,0,0])
cylinder(h=50,d=50);

translate([-10,-52.5,27])
cube([20,20,5]);
}

}
//slide mounts
translate([-7.5,-50,0])
cube([15,15,50]);

translate([-7.5,-110,0])
cube([15,15,50]);

//hollow the core
union()
{
translate([0,1,0])
rotate([90,0,0])
cylinder(h=27,d1=70,d2=45);

rotate([90,0,0])
cylinder(h=128,d=45);
}

//leg screws
leg_screws();

translate([0,-60,0])
leg_screws();



}


screw_holes();

module leg_screws()
{
translate([-50,-42,27.5])
rotate([0,90,0])
cylinder(h=100,d=screw_hole_dia);


//leg screw counter sink
translate([-50,-42,27.5])
rotate([0,90,0])
cylinder(h=40,d=9);

translate([10,-42,27.5])
rotate([0,90,0])
cylinder(h=40,d=9);
}


module screw_holes()
{
rotate([0,45,0])
union()
{
//top
translate([0,-112.5,21])
rotate([90,0,0])
screw_set();
//left  
translate([21,-112.5,0])
rotate([90,0,0])
screw_set();
//right
translate([-21,-112.5,0])
rotate([90,0,0])
screw_set();
//bottom 
translate([0,-112.5,-21])
rotate([90,0,0])
screw_set();
}


rotate([0,45,0])
union()
{
//top
translate([0,-112.5,21])
rotate([90,180,0])
screw_taper();
//left  
translate([21,-112.5,0])
rotate([90,-90,0])
screw_taper();
//right
translate([-21,-112.5,0])
rotate([90,90,0])
screw_taper();
//bottom 
translate([0,-112.5,-21])
rotate([90,0,0])
screw_taper();
}



// big side

intersection()
{
union()
{
//top
translate([0,0,32.5])
rotate([90,0,0])
screw_set();
//left  
translate([32.5,0,0])
rotate([90,0,0])
screw_set();
//right
translate([-32.5,0,0])
rotate([90,0,0])
screw_set();
//bottom 
translate([0,0,-32.5])
rotate([90,0,0])
screw_set();
}
rotate([90,0,0])
cylinder(h=25,d1=75,d2=50);
}

}

module screw_taper()
{
hull()
{
cylinder(h=1,d=5);
translate([0,-2,-5])
cylinder(h=1,d=1);
}
}

module screw_set()
{
difference()
{
cylinder(h=15,d=5);
translate([0,0,-1])
cylinder(h=17,d=screw_hole_dia);
}

}

module dupont_6_pack()
{
for(a=[0:2.54:2.54])
{

for(b=[0:2.54:5.08])
{
translate([b,a,0])
dupont_single();
}
}
}



module dupont_single()
{
difference()
{
translate([-1.27,-1.27,0])
cube([2.54,2.54,14]);
translate([0,0,-1])
cylinder(h=16,d=2);
}
}

module thumb_joy_blank()
{
translate([-13,-13,0])
cube([26,26,3]);

cylinder(h=25,d=25);
}

