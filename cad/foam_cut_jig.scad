$fn=360;

PVC_OD=62; //.5 larger than retainer to allow motion
FOAM_OD=62;
lock_thickness=80; // 80 +2 for base
Wall_thickness=5;
screw_diameter=4.8;




difference()
{
  
        cylinder(h=lock_thickness+Wall_thickness,d=PVC_OD+5);

     
        
translate([0,0,Wall_thickness])
        cylinder(h=lock_thickness+1,d=PVC_OD);
        
translate([0,0,-1])
        cylinder(h=lock_thickness,d=PVC_OD-5);
}        

difference()
{
union()
{
translate([-31,-2.5,0])
cube([62,5,Wall_thickness]);

translate([-2.5, -31,0])
cube([5,62,Wall_thickness]);
}

translate([0,0,-1])
cylinder(h=Wall_thickness+2,d=30);
}

difference()
{
translate([0,0,0])
cylinder(h=Wall_thickness,d=32);

translate([0,0,-1])
cylinder(h=Wall_thickness+2,d=22);
}

