$fn=360;

/* Tank boat link starting at 22mm in ID
5 mm thick walls and M5 set screw
*/



PVC_OD=22; //.5 larger than retainer to allow motion
FOAM_OD=60;
lock_thickness=5; // links are 5mm thick
Wall_thickness=5;
screw_diameter=4.8;

difference()
{
    union()
    {
        hull()
        {
        cylinder(h=lock_thickness,d=FOAM_OD);
        translate([75,0,0])
        cylinder(h=lock_thickness,d=PVC_OD);
        }

        hull()
        {
        translate([75,0,0])
        cylinder(h=lock_thickness,d=FOAM_OD);
        translate([0,0,0])
        cylinder(h=lock_thickness,d=PVC_OD);
        }
    }


translate([0,0,-1])
cylinder(h=lock_thickness+2,d=PVC_OD);

translate([75,0,-1])
cylinder(h=lock_thickness+2,d=PVC_OD);
}




