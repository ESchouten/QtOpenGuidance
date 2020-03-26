// Copyright( C ) 2020 Christian Riggenbach
//
// This program is free software:
// you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// ( at your option ) any later version.
//
// This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY;
// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see < https : //www.gnu.org/licenses/>.

#include "LocalPlanner.h"

#include "../cgal.h"

void LocalPlanner::setPose(const Point_3& position, QQuaternion orientation, PoseOption::Options options) {
  if( !options.testFlag( PoseOption::CalculateLocalOffsets ) ) {
    this->position = position;
    this->orientation = orientation;

    const Point_2 position2D = to2D( position );

    if( !globalPlan.plan->empty() ) {

      // local planner for lines: find the nearest line and put it into the local plan
      double distanceSquared = qInf();
      std::shared_ptr<PathPrimitive> nearestPrimitive = nullptr;


      for( const auto& pathPrimitive : *globalPlan.plan ) {
        double currentDistanceSquared = pathPrimitive->distanceToPointSquared( position2D );

        if( currentDistanceSquared < distanceSquared ) {
          nearestPrimitive = pathPrimitive;
          distanceSquared = currentDistanceSquared;
        } else {
          if( globalPlan.type == Plan::Type::OnlyLines ) {
            // the plan is ordered, so we can take the fast way out...
            break;
          }
        }
      }

      plan.type = Plan::Type::OnlyLines;
      plan.plan->clear();

      if( nearestPrimitive->anyDirection ) {
        double angleNearestPrimitive = nearestPrimitive->angleAtPoint( position2D );

        if( std::abs( orientation.toEulerAngles().z() - angleNearestPrimitive ) > 95 ) {
          nearestPrimitive = nearestPrimitive->createReverse();
        }

        plan.plan->push_back( nearestPrimitive );
        emit planChanged( plan );
      }
    }
  }
}
