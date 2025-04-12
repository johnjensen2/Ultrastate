$fn=360;

fender_washer_OD=38;
fender_washer_ID=10;
number_of_holes=10;
hole_dia=3;




difference()
{

cylinder(h=6,d=45);

    //washer outline
    translate([0,0,5])
    cylinder(h=5,d=fender_washer_OD);

    //center bot hole
    translate([0,0,-1])
    cylinder(h=9,d=8);

    //outter drill holes
    for(a=[0:360/number_of_holes:360])
    {
    rotate([0,0,a])

    translate([fender_washer_OD/2-hole_dia-2,0,-1])
    cylinder(h=7,d=hole_dia);
    }


}

//post to align
difference()
{
translate([0,0,0])
cylinder(h=8,d=fender_washer_ID);

translate([0,0,-1])
cylinder(h=11,d=8);

}

