$fn=360;

/* Tank boat link starting at 22mm in ID
 spacing check for size
*/


PVC_OD=22;

difference()
{
cylinder(h=70,d=PVC_OD+5);
translate([0,0,-1])
cylinder(h=66,d=PVC_OD);
}
