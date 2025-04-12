//base cover
$fn=360; // Smooth cylinders

// Define the cube dimensions
cube_length = 150;
cube_height = 120;
cube_depth = 35;
cube_wall =2.5;
hole_diameter = 3;
pinwall=5;
lmu_dia=15;
lmu_height=24;
lmu_shaft=9; //slightly larger that its real 8mm
lmu_shaft_height2=160;

lmu_shaft_height=50;
base_height=5;
top_height=3;

section_height=2;
wire_hole_dia=25;

screw_cylinder=6;
screw_dia=2.8;

plate_thickness=2.5;

difference()
{
 plates();
 
 screw_cylinders();
}


module plates()
{
translate([0,0,3])
cube([40,120,plate_thickness]);

translate([125,0,3])
cube([25,120,plate_thickness]);

translate([0,0,3])
cube([150,20,plate_thickness]);

translate([0,100,3])
cube([150,25,plate_thickness]);

translate([0,40,3])
cube([150,40,plate_thickness]);
}





module screw_cylinders()
{
//screw holes for cover
for(a=[5:20:150])
{
translate([a,5,0])

    translate([0,0,-1])
    cylinder(h=cube_depth,d=screw_dia);
  
}

for(a=[5:20:150])
{
translate([a,115,0])

    translate([0,0,-1])
    cylinder(h=cube_depth,d=screw_dia);
    
}

for(a=[30:30:100])
{
translate([5,a,0])

    translate([0,0,-1])
    cylinder(h=cube_depth,d=screw_dia);

}

for(a=[30:30:100])
{
translate([145,a,0])

    translate([0,0,-1])
    cylinder(h=cube_depth,d=screw_dia);
    
}

}








module outer_pins()
{
difference()
{
union()
{
translate([25,-5,-5])
rotate([90,0,180])
section(45);

translate([75,-5,-5])
rotate([90,0,180])
section(45);

translate([125,-5,-5])
rotate([90,0,180])
section(45);
}
 translate([0, -10, -15])
    cube([cube_length, cube_height+pinwall, 15]);
    
translate([0, -10, cube_depth])
    cube([cube_length, cube_height+pinwall, 15]);    
}
}
module slide_shaft()
{
translate([-1,45,19])
rotate([0,90,0])
cylinder(h=lmu_shaft_height2,d=lmu_shaft);
translate([-1,75,19])
rotate([0,90,0])
cylinder(h=lmu_shaft_height2,d=lmu_shaft);
}