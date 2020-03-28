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

#pragma once

#include <QObject>

#include <QQuaternion>
#include <QVector3D>

#include "BlockBase.h"

#include "qneblock.h"
#include "qneport.h"

#include "../cgalKernel.h"
#include "../kinematic/PoseOptions.h"
#include "../kinematic/PathPrimitive.h"
#include "../kinematic/Plan.h"

#include "../gui/GuidanceTurning.h"

#include "../gui/MyMainWindow.h"
#include <kddockwidgets/KDDockWidgets.h>
#include <kddockwidgets/DockWidget.h>


#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QExtrudedTextMesh>
#include "../3d/BufferMesh.h"

class LocalPlanner : public BlockBase {
    Q_OBJECT

  public:
    explicit LocalPlanner( const QString& uniqueName,
                           MyMainWindow* mainWindow,
                           Qt3DCore::QEntity* rootEntity
                         )
      : BlockBase(),
        rootEntity( rootEntity ) {
      widget = new GuidanceTurning( mainWindow );
      dock = new KDDockWidgets::DockWidget( uniqueName );

      QObject::connect( widget, &GuidanceTurning::turnLeftToggled, this, &LocalPlanner::turnLeftToggled );
      QObject::connect( widget, &GuidanceTurning::turnRightToggled, this, &LocalPlanner::turnRightToggled );
      QObject::connect( widget, &GuidanceTurning::numSkipChanged, this, &LocalPlanner::numSkipChanged );


      // test for recording
      {
        m_baseEntity = new Qt3DCore::QEntity( rootEntity );
        m_baseTransform = new Qt3DCore::QTransform( m_baseEntity );
        m_baseEntity->addComponent( m_baseTransform );

        m_segmentsEntity = new Qt3DCore::QEntity( m_baseEntity );

        m_segmentsMesh = new BufferMesh( m_segmentsEntity );
        m_segmentsMesh->setPrimitiveType( Qt3DRender::QGeometryRenderer::Lines );
        m_segmentsEntity->addComponent( m_segmentsMesh );

        m_segmentsMaterial = new Qt3DExtras::QPhongMaterial( m_segmentsEntity );
        m_segmentsMaterial->setAmbient( Qt::red );
        m_segmentsEntity->addComponent( m_segmentsMaterial );
      }

    }

    ~LocalPlanner() {
      dock->deleteLater();
      widget->deleteLater();
    }

  public slots:
    void setPose( const Point_3& position, QQuaternion orientation, PoseOption::Options options );

    void setPlan( const Plan& plan ) {
      this->globalPlan = plan;
//      emit planChanged( plan );
    }

    void turnLeftToggled( bool state );
    void turnRightToggled( bool state );
    void numSkipChanged( int left, int right );


  signals:
    void planChanged( const Plan& );
    void triggerPlanPose( const Point_3& position, QQuaternion orientation, PoseOption::Options options );

  public:
    Point_3 position = Point_3( 0, 0, 0 );
    QQuaternion orientation = QQuaternion();

    GuidanceTurning* widget = nullptr;
    KDDockWidgets::DockWidget* dock = nullptr;

  private:
    Plan::ConstPrimitiveIterator getNearestPrimitive( const Point_2& position2D, Plan plan, double& distanceSquared );

    Plan globalPlan;
    Plan plan;

    bool turningLeft = false;
    bool turningRight = false;
    int leftSkip = 1;
    int rightSkip = 1;
    Point_3 positionTurnStart = Point_3( 0, 0, 0 );
    Segment_2 targetSegment = Segment_2( Point_2( 0, 0 ), Point_2( 0, 0 ) );





    Qt3DCore::QEntity* rootEntity = nullptr;
    Qt3DCore::QEntity* m_baseEntity = nullptr;
    Qt3DCore::QTransform* m_baseTransform = nullptr;
    Qt3DCore::QEntity* m_segmentsEntity = nullptr;
    BufferMesh* m_segmentsMesh = nullptr;
    Qt3DExtras::QPhongMaterial* m_segmentsMaterial = nullptr;
};

class LocalPlannerFactory : public BlockFactory {
    Q_OBJECT

  public:
    LocalPlannerFactory( MyMainWindow* mainWindow,
                         KDDockWidgets::Location location,
                         QMenu* menu,
                         Qt3DCore::QEntity* rootEntity )
      : BlockFactory(),
        mainWindow( mainWindow ),
        location( location ),
        menu( menu ),
        rootEntity( rootEntity ) {}

    QString getNameOfFactory() override {
      return QStringLiteral( "Local Planner" );
    }

    virtual QNEBlock* createBlock( QGraphicsScene* scene, int id ) override {
      auto* object = new LocalPlanner( getNameOfFactory() + QString::number( id ),
                                       mainWindow,
                                       rootEntity );
      auto* b = createBaseBlock( scene, object, id );

      object->dock->setTitle( getNameOfFactory() );
      object->dock->setWidget( object->widget );

      menu->addAction( object->dock->toggleAction() );

      mainWindow->addDockWidget( object->dock, location );

      b->addInputPort( QStringLiteral( "Pose" ), QLatin1String( SLOT( setPose( const Point_3&, const QQuaternion, const PoseOption::Options ) ) ) );
      b->addInputPort( QStringLiteral( "Plan" ), QLatin1String( SLOT( setPlan( const Plan& ) ) ) );
      b->addOutputPort( QStringLiteral( "Trigger Plan Pose" ), QLatin1String( SIGNAL( triggerPlanPose( const Point_3&, const QQuaternion, const PoseOption::Options ) ) ) );
      b->addOutputPort( QStringLiteral( "Plan" ), QLatin1String( SIGNAL( planChanged( const Plan& ) ) ) );

      return b;
    }

  private:
    MyMainWindow* mainWindow = nullptr;
    KDDockWidgets::Location location;
    QMenu* menu = nullptr;
    Qt3DCore::QEntity* rootEntity = nullptr;
};
