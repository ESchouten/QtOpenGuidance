// Copyright( C ) 2019 Christian Riggenbach
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

#include "GuidanceGlobalPlanner.h"
#include <QtConcurrent>

#include "../cgal.h"

void GlobalPlanner::createPlanAB() {
//  if( !qIsNull( abLine.squared_lenght() ) ) {

//    QVector<QVector3D> linePoints;

//    // extend the points 200m in either direction
//    qreal headingOfABLine = abLine.angle();

//    QLineF lineExtensionFromA = QLineF::fromPolar( -200, headingOfABLine );
//    lineExtensionFromA.translate( aPoint );

//    QLineF lineExtensionFromB = QLineF::fromPolar( 200, headingOfABLine );
//    lineExtensionFromB.translate( bPoint );

//    QLineF pathLine( lineExtensionFromA.p2(), lineExtensionFromB.p2() );

//    QVector<QSharedPointer<PathPrimitive>> plan;

//    // the lines are generated to follow in both directions
//    if( forwardPasses == 0 && reversePasses == 0 ) {
//      // left side
//      for( uint16_t i = 0; i < pathsToGenerate; ++i ) {
//        plan.append( QSharedPointer<PathPrimitive>(
//                       new PathPrimitiveLine(
//                         pathLine.translated(
//                           QLineF::fromPolar( i * implementLine.dy() +
//                                              implementLine.center().y(),
//                                              headingOfABLine - 90 ).p2() ), implementLine.dy(), false, true, i ) ) );
//      }

//      // right side
//      for( uint16_t i = 1; i < pathsToGenerate; ++i ) {
//        plan.append( QSharedPointer<PathPrimitive>(
//                       new PathPrimitiveLine(
//                         pathLine.translated(
//                           QLineF::fromPolar( i * implementLine.dy() +
//                                              implementLine.center().y(),
//                                              headingOfABLine - 270 ).p2() ), implementLine.dy(), false, true, -i ) ) );
//      }

//      // these lines are to follow in the generated direction
//    } else {
//      // add center line
//      auto linePrimitive = new PathPrimitiveLine(
//        pathLine.translated(
//          QLineF::fromPolar( implementLine.center().y(),
//                             headingOfABLine + 90 ).p2() ), implementLine.dy(), false, false, 0 );
//      plan.append( QSharedPointer<PathPrimitive>( linePrimitive ) );

//      enum class PathGenerationStates : uint8_t {
//        Forward = 0,
//        Reverse,
//        ForwardMirrored,
//        ReverseMirrored
//      } pathGenerationStates = PathGenerationStates::Forward;

//      uint16_t passCounter = 1;

//      // left side
//      if( !startRight ) {
//        if( forwardPasses == 1 ) {
//          pathGenerationStates = PathGenerationStates::Reverse;
//        } else {
//          pathGenerationStates = PathGenerationStates::Forward;
//        }
//      } else {
//        if( mirror ) {
//          pathGenerationStates = PathGenerationStates::ForwardMirrored;
//        } else {
//          pathGenerationStates = PathGenerationStates::Reverse;
//          passCounter = 0;
//        }

//      }

//      for( uint16_t i = 1; i < pathsToGenerate; ++i ) {

//        auto linePrimitive = new PathPrimitiveLine(
//          pathLine.translated(
//            QLineF::fromPolar( i * implementLine.dy() +
//                               implementLine.center().y(),
//                               headingOfABLine + 90 ).p2() ), implementLine.dy(), false, false, i );

//        ++passCounter;

//        // state machine to generate the directions of the lines
//        switch( pathGenerationStates ) {
//          case PathGenerationStates::Forward:
//            if( passCounter >= forwardPasses ) {
//              pathGenerationStates = PathGenerationStates::Reverse;
//              passCounter = 0;
//            }

//            break;

//          case PathGenerationStates::Reverse:
//            linePrimitive->reverse();

//            if( passCounter >= reversePasses ) {
//              if( mirror ) {
//                pathGenerationStates = PathGenerationStates::ReverseMirrored;
//              } else {
//                pathGenerationStates = PathGenerationStates::Forward;
//              }

//              passCounter = 0;
//            }

//            break;

//          case PathGenerationStates::ForwardMirrored:
//            linePrimitive->reverse();

//            if( passCounter >= forwardPasses ) {
//              pathGenerationStates = PathGenerationStates::Forward;
//              passCounter = 0;
//            }

//            break;

//          case PathGenerationStates::ReverseMirrored:
//            if( passCounter >= reversePasses ) {
//              if( mirror ) {
//                pathGenerationStates = PathGenerationStates::ForwardMirrored;
//              } else {
//                pathGenerationStates = PathGenerationStates::Forward;
//              }

//              passCounter = 0;
//            }

//            break;
//        }

//        plan.append( QSharedPointer<PathPrimitive>( linePrimitive ) );
//      }


//      // right side
//      passCounter = 1;

//      if( startRight ) {
//        if( forwardPasses == 1 ) {
//          pathGenerationStates = PathGenerationStates::Reverse;
//        } else {
//          pathGenerationStates = PathGenerationStates::Forward;
//        }
//      } else {
//        if( mirror ) {
//          pathGenerationStates = PathGenerationStates::ForwardMirrored;
//        } else {
//          pathGenerationStates = PathGenerationStates::Reverse;
//        }

//        passCounter = 0;
//      }

//      for( uint16_t i = 1; i < pathsToGenerate; ++i ) {

//        auto linePrimitive = new PathPrimitiveLine(
//          pathLine.translated(
//            QLineF::fromPolar( i * implementLine.dy() +
//                               implementLine.center().y(),
//                               headingOfABLine - 90 ).p2() ), implementLine.dy(), false, false, i );

//        ++passCounter;

//        // state machine to generate the directions of the lines
//        switch( pathGenerationStates ) {
//          case PathGenerationStates::Forward:
//            if( passCounter >= forwardPasses ) {
//              pathGenerationStates = PathGenerationStates::Reverse;
//              passCounter = 0;
//            }

//            break;

//          case PathGenerationStates::Reverse:
//            linePrimitive->reverse();

//            if( passCounter >= reversePasses ) {
//              if( mirror ) {
//                pathGenerationStates = PathGenerationStates::ReverseMirrored;
//              } else {
//                pathGenerationStates = PathGenerationStates::Forward;
//              }

//              passCounter = 0;
//            }

//            break;

//          case PathGenerationStates::ForwardMirrored:
//            linePrimitive->reverse();

//            if( passCounter >= forwardPasses ) {
//              pathGenerationStates = PathGenerationStates::Forward;
//              passCounter = 0;
//            }

//            break;

//          case PathGenerationStates::ReverseMirrored:
//            if( passCounter >= reversePasses ) {
//              if( mirror ) {
//                pathGenerationStates = PathGenerationStates::ForwardMirrored;
//              } else {
//                pathGenerationStates = PathGenerationStates::Forward;
//              }

//              passCounter = 0;
//            }

//            break;
//        }

//        plan.append( QSharedPointer<PathPrimitive>( linePrimitive ) );
//      }

//    }

//    emit planChanged( plan );
//  }
}

GlobalPlanner::GlobalPlanner( Qt3DCore::QEntity* rootEntity, TransverseMercatorWrapper* tmw )
  : BlockBase(),
    tmw( tmw ) {
  // a point marker -> orange
  {
    aPointMesh = new Qt3DExtras::QSphereMesh();
    aPointMesh->setRadius( .2f );
    aPointMesh->setSlices( 20 );
    aPointMesh->setRings( 20 );

    aPointTransform = new Qt3DCore::QTransform();

    Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse( QColor( "orange" ) );

    aPointEntity = new Qt3DCore::QEntity( rootEntity );
    aPointEntity->addComponent( aPointMesh );
    aPointEntity->addComponent( material );
    aPointEntity->addComponent( aPointTransform );
    aPointEntity->setEnabled( false );

    aTextEntity = new Qt3DCore::QEntity( aPointEntity );
    Qt3DExtras::QExtrudedTextMesh* aTextMesh = new Qt3DExtras::QExtrudedTextMesh();
    aTextMesh->setText( QStringLiteral( "A" ) );
    aTextMesh->setDepth( 0.05f );

    aTextEntity->setEnabled( true );
    aTextTransform = new Qt3DCore::QTransform();
    aTextTransform->setRotation( QQuaternion::fromAxisAndAngle( QVector3D( 0, 0, 1 ), -90 ) );
    aTextTransform->setScale( 2.0f );
    aTextTransform->setTranslation( QVector3D( 0, -.2f, 0 ) );
    aTextEntity->addComponent( aTextTransform );
    aTextEntity->addComponent( aTextMesh );
    aTextEntity->addComponent( material );
  }

  // b point marker -> purple
  {
    bPointMesh = new Qt3DExtras::QSphereMesh();
    bPointMesh->setRadius( .2f );
    bPointMesh->setSlices( 20 );
    bPointMesh->setRings( 20 );

    bPointTransform = new Qt3DCore::QTransform();

    Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse( QColor( "purple" ) );

    bPointEntity = new Qt3DCore::QEntity( rootEntity );
    bPointEntity->addComponent( bPointMesh );
    bPointEntity->addComponent( material );
    bPointEntity->addComponent( bPointTransform );
    bPointEntity->setEnabled( false );

    bTextEntity = new Qt3DCore::QEntity( bPointEntity );
    Qt3DExtras::QExtrudedTextMesh* bTextMesh = new Qt3DExtras::QExtrudedTextMesh();
    bTextMesh->setText( QStringLiteral( "B" ) );
    bTextMesh->setDepth( 0.05f );

    bTextEntity->setEnabled( true );
    bTextTransform = new Qt3DCore::QTransform();
    bTextTransform->setRotation( QQuaternion::fromAxisAndAngle( QVector3D( 0, 0, 1 ), -90 ) );
    bTextTransform->setScale( 2.0f );
    bTextTransform->setTranslation( QVector3D( 0, -.2f, 0 ) );
    bTextEntity->addComponent( bTextTransform );
    bTextEntity->addComponent( bTextMesh );
    bTextEntity->addComponent( material );
  }

  // test for recording
  {
    m_baseEntity = new Qt3DCore::QEntity( rootEntity );
    m_baseTransform = new Qt3DCore::QTransform();
    m_baseEntity->addComponent( m_baseTransform );

    m_pointsEntity = new Qt3DCore::QEntity( m_baseEntity );
    m_segmentsEntity = new Qt3DCore::QEntity( m_baseEntity );
    m_segmentsEntity2 = new Qt3DCore::QEntity( m_baseEntity );
    m_segmentsEntity3 = new Qt3DCore::QEntity( m_baseEntity );
    m_segmentsEntity4 = new Qt3DCore::QEntity( m_baseEntity );
    m_segmentsEntity->setEnabled( false );
    m_segmentsEntity2->setEnabled( false );
    m_segmentsEntity3->setEnabled( false );

    m_pointsMesh = new BufferMesh();
    m_pointsMesh->setPrimitiveType( Qt3DRender::QGeometryRenderer::Points );
    m_pointsEntity->addComponent( m_pointsMesh );

    m_segmentsMesh = new BufferMesh();
    m_segmentsMesh->setPrimitiveType( Qt3DRender::QGeometryRenderer::LineStrip );
    m_segmentsEntity->addComponent( m_segmentsMesh );

    m_segmentsMesh2 = new BufferMesh();
    m_segmentsMesh2->setPrimitiveType( Qt3DRender::QGeometryRenderer::LineStrip );
    m_segmentsEntity2->addComponent( m_segmentsMesh2 );

    m_segmentsMesh3 = new BufferMesh();
    m_segmentsMesh3->setPrimitiveType( Qt3DRender::QGeometryRenderer::LineStrip );
    m_segmentsEntity3->addComponent( m_segmentsMesh3 );

    m_segmentsMesh4 = new BufferMesh();
    m_segmentsMesh4->setPrimitiveType( Qt3DRender::QGeometryRenderer::LineStrip );
    m_segmentsEntity4->addComponent( m_segmentsMesh4 );

    m_pointsMaterial = new Qt3DExtras::QPhongMaterial( m_pointsEntity );
    m_segmentsMaterial = new Qt3DExtras::QPhongMaterial( m_segmentsEntity );
    m_segmentsMaterial2 = new Qt3DExtras::QPhongMaterial( m_segmentsEntity2 );
    m_segmentsMaterial3 = new Qt3DExtras::QPhongMaterial( m_segmentsEntity3 );
    m_segmentsMaterial4 = new Qt3DExtras::QPhongMaterial( m_segmentsEntity4 );

    m_pointsMaterial->setAmbient( Qt::yellow );
    m_segmentsMaterial->setAmbient( Qt::white );
    m_segmentsMaterial2->setAmbient( Qt::green );
    m_segmentsMaterial3->setAmbient( Qt::blue );
    m_segmentsMaterial4->setAmbient( Qt::red );

    m_pointsEntity->addComponent( m_pointsMaterial );
    m_segmentsEntity->addComponent( m_segmentsMaterial );
    m_segmentsEntity2->addComponent( m_segmentsMaterial2 );
    m_segmentsEntity3->addComponent( m_segmentsMaterial3 );
    m_segmentsEntity4->addComponent( m_segmentsMaterial4 );
  }

  // create the CGAL worker and move to it's own thread
  {
    threadForCgalWorker = new QThread();
    cgalWorker = new CgalWorker();

    qRegisterMetaType<FieldsOptimitionToolbar::AlphaType>();

    QObject::connect( threadForCgalWorker, &QThread::finished, cgalWorker, &CgalWorker::deleteLater );

    QObject::connect( this, &GlobalPlanner::requestFieldOptimition, cgalWorker, &CgalWorker::fieldOptimitionWorker );
    QObject::connect( cgalWorker, &CgalWorker::alphaChanged, this, &GlobalPlanner::alphaChanged );
    QObject::connect( cgalWorker, &CgalWorker::fieldStatisticsChanged, this, &GlobalPlanner::fieldStatisticsChanged );
    QObject::connect( cgalWorker, &CgalWorker::alphaShapeFinished, this, &GlobalPlanner::alphaShapeFinished );

    cgalWorker->moveToThread( threadForCgalWorker );
    threadForCgalWorker->start();
  }
}

void GlobalPlanner::alphaShape() {
  QElapsedTimer timer;
  timer.start();

  // you need at least 3 points for area
  if( points.size() >= 3 ) {

    // make a 2D copy of the recorded points
    auto pointsCopy2D = new std::vector<K::Point_2>();
    pointsCopy2D->reserve( points.size() );

    for( const auto& point : points ) {
      pointsCopy2D->emplace_back( point.x(), point.y() );
    }

    emit requestFieldOptimition( pointsCopy2D,
                                 alphaType,
                                 customAlpha,
                                 maxDeviation,
                                 distanceBetweenConnectPoints );
  }

  qDebug() << "GlobalPlanner::alphaShape: " << timer.nsecsElapsed() << "ns";
}

void GlobalPlanner::alphaShapeFinished( Polygon_with_holes_2* out_poly ) {
  qDebug() << "GlobalPlanner::alphaShapeFinished" << out_poly;

  QVector<QVector3D> meshSegmentPoints;
  typedef Polygon_2::Vertex_iterator VertexIterator;

  for( VertexIterator vi = out_poly->outer_boundary().vertices_begin(),
       end = out_poly->outer_boundary().vertices_end();
       vi != end; ++vi ) {
    meshSegmentPoints << QVector3D( float( vi->x() ), float( vi->y() ), 0.1f );
  }

  meshSegmentPoints << meshSegmentPoints.first();
  m_segmentsMesh4->bufferUpdate( meshSegmentPoints );

  delete out_poly;
}