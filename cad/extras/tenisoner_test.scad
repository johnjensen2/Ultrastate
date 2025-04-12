$fn=360;

tenisoner();



module tenisoner()
{
    difference()
    {
        translate([15,0,-1])
        cylinder(h=3,d=20);


        translate([17,0,-5])
        cylinder(h=15,d=5);

        translate([17,0,0])
        cylinder(h=5,d=10);

        hull()
        {
        translate([20,25,1])
        rotate([90,0,0])
        cylinder(h=50,d=2);

        translate([20,25,4])
        rotate([90,0,0])
        cylinder(h=50,d=2);
        }

    }
}