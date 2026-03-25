using System.Collections.Generic;
namespace tw_hw_6_UI.Models
{
    public class NeedleThrow
    {
        public double CenterX { get; set; }                // горизонтальная координата центра иглы
        public double DistanceToNearestLine { get; set; }  // расстояние до ближайшей прямой
        public bool NearestIsBottom { get; set; }          // true - ближайшая нижняя, false - верхняя
        public double Angle { get; set; }                  // угол между иглой и горизонталью (0..π/2)
        public bool CrossesLine { get; set; }              // пересекает ли игла прямую
    }
}