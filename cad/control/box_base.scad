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

liner_pot();

//translate([30,90,19])
//tray();



module liner_pot()
{
body();


}

module screw_cylinders()
{
//screw holes for cover
for(a=[5:20:150])
{
translate([a,5,0])
    difference()
    {
    cylinder(h=cube_depth,d=screw_cylinder);
    translate([0,0,cube_wall])
    cylinder(h=cube_depth,d=screw_dia);
    }
}

for(a=[5:20:150])
{
translate([a,115,0])
    difference()
    {
    cylinder(h=cube_depth,d=screw_cylinder);
    translate([0,0,cube_wall])
    cylinder(h=cube_depth,d=screw_dia);
    }
}

for(a=[30:30:100])
{
translate([5,a,0])
    difference()
    {
    cylinder(h=cube_depth,d=screw_cylinder);
    translate([0,0,cube_wall])
    cylinder(h=cube_depth,d=screw_dia);
    }
}

for(a=[30:30:100])
{
translate([145,a,0])
    difference()
    {
    cylinder(h=cube_depth,d=screw_cylinder);
    translate([0,0,cube_wall])
    cylinder(h=cube_depth,d=screw_dia);
    }
}

}
module wire_holes()
{
translate([50,130,15])
rotate([90,0,0])
cylinder(h=cube_height+20,d=wire_hole_dia);

translate([100,130,15])
rotate([90,0,0])
cylinder(h=cube_height+20,d=wire_hole_dia);
}


module pin_slots()
{
translate([25,120,-5])
rotate([90,0,180])
section(45);

translate([75,120,-5])
rotate([90,0,180])
section(45);

translate([125,120,-5])
rotate([90,0,180])
section(45);
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
translate([-1,15,19])
rotate([0,90,0])
cylinder(h=lmu_shaft_height2,d=lmu_shaft);
translate([-1,105,19])
rotate([0,90,0])
cylinder(h=lmu_shaft_height2,d=lmu_shaft);
}

module body()
{
difference()
{
    union()
    {
    translate([0, 0, 0])
    cube([cube_length, cube_height+pinwall, cube_depth]);

    outer_pins();
    }
    
    translate([cube_wall, cube_wall, cube_wall])
    cube([cube_length-cube_wall*2, cube_height-cube_wall*2, cube_depth]); 
 
slide_shaft();
 pin_slots();
// wire_holes();


}


}
 
 
 module stand_off()
 {
 difference()
 {
  translate([0, 0,0])
 cylinder(h=5,d1=8,d2=5);
   translate([0, 0,-1])
cylinder(h=11,d=hole_diameter);
 }
 }
 
 module stand_offs()
 {
//stand offs
 translate([10, 10, cube_wall])
 cylinder(h=5,d1=8,d2=5);
 
 
 translate([10, 30, cube_wall])
 cylinder(h=5,d1=8,d2=5);
 
 
 translate([95, 10, cube_wall])
 cylinder(h=5,d1=8,d2=5);
 
 
 translate([95, 30, cube_wall])
 cylinder(h=5,d1=8,d2=5);
 
 }
 
   
module holes()
{
 translate([10, 10,cube_wall])
 cylinder(h=11,d=hole_diameter);
 
 
 translate([10, 30,cube_wall])
  cylinder(h=11,d=hole_diameter);
 
 
 translate([95, 10,cube_wall])
  cylinder(h=11,d=hole_diameter);
 
 
 translate([95, 30,cube_wall])
  cylinder(h=11,d=hole_diameter);
 
 }
 


module tray()
{
difference()
{
//base for now
translate([-(lmu_height+5)/2,-70,0])
cube([lmu_height+5,80,15]);

//bearings
translate([-12,-15,0])
rotate([0,90,0])
cylinder(h=lmu_height,d=lmu_dia);

translate([-12,-45,0])
rotate([0,90,0])
cylinder(h=lmu_height,d=lmu_dia);

//shaft holes
translate([-20,15,0])
rotate([0,90,0])
cylinder(h=lmu_shaft_height,d=lmu_shaft);

translate([-20,-75,0])
rotate([0,90,0])
cylinder(h=lmu_shaft_height,d=lmu_shaft);


//stem holes
translate([0,0,5])
stem();

translate([0,-60,5])
stem();

//wiring 
translate([-2.54,-1.27,-1])
dupont_6_pack();

translate([-2.54,-61.27,-1])
dupont_6_pack();

//zip ties holes
zip_ties();
translate([0,-30,0])
zip_ties();

//linear_pot hole
translate([-2,-30.6,-1])
cube([4,1.2,20]);

//linear pot groove 

translate([-25,-32.5,-1])
cube([50,5,6]);
}
}

module zip_ties()
{
translate([8.5,-6.5,-1])
cube([5,3,20]);

translate([-13.5,-6.5,-1])
cube([5,3,20]);

translate([8.5,-11.5-lmu_dia,-1])
cube([5,3,20]);

translate([-13.5,-11.5-lmu_dia,-1])
cube([5,3,20]);
}

module stem()
{
hull()
{
translate([6,6,0])
cylinder(h=55,d=3);

translate([-6,6,0])
cylinder(h=55,d=3);

translate([6,-6,0])
cylinder(h=55,d=3);

translate([-6,-6,0])
cylinder(h=55,d=3);
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

module section(a)
{

    hull()
    {
    cylinder(h=section_height,d=base_height);

    translate([0,a,0])
    cylinder(h=section_height,d=base_height);
    }
    
    hull()
    {
    translate([0,0,section_height])
    cylinder(h=section_height,d1=base_height,d2=top_height);

    translate([0,a,section_height])
    cylinder(h=section_height,d1=base_height,d2=top_height);
    }

    hull()
    {
    translate([0,0,section_height*2])
    cylinder(h=section_height+5,d=top_height);
    
    translate([0,a,section_height*2])
    cylinder(h=section_height+5,d=top_height);
    }


    
    
 }

module dupont_single()
{
difference()
{
translate([-1.27,-1.27,0])
cube([2.54,2.54,15]);
//don't need the holes in this
//translate([0,0,-1])
//cylinder(h=16,d=2);
}
}

 module text()
 {
 
//label text min
translate([82.5,10,-1])
mirror([0,1,0])
rotate([0,0,90])
//text_a("MIN");
linear_extrude(3)
text("MIN",size=3);

//label text max
translate([15,10,-1])
mirror([0,1,0])
rotate([0,0,90])
//text_a("MIN");
linear_extrude(3)
text("MAX",size=3);
}
 