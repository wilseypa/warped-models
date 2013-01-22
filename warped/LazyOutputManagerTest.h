#ifndef LAZY_OUTPUT_MANAGER_TEST_H
#define LAZY_OUTPUT_MANAGER_TEST_H

// Copyright (c) 2003 Clifton Labs, Inc.  All rights reserved.

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, RESULT
// OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.

// Authors: Dale E. Martin              dmartin@cliftonlabs.com
//          Randall King                kingr8@cliftonlabs.com

#include "OutputManagerTest.h"
#include "warped/LazyOutputManager.h"

class LazyOutputManager;

class LazyOutputManagerTest : public OutputManagerTest {
  CPPUNIT_TEST_SUITE( LazyOutputManagerTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testInsert );
  CPPUNIT_TEST( testRollback );
  CPPUNIT_TEST( testFossilCollect );
  CPPUNIT_TEST( testFossilCollectInt );
  CPPUNIT_TEST( testOfcPurge );
  CPPUNIT_TEST( testLazyCancel );
  CPPUNIT_TEST_SUITE_END();

  class LOMSimMgr : public UnitTestSimulationManager {
  public:
    LOMSimMgr( SimulationObject *initObj ) :
      UnitTestSimulationManager( initObj ){}
    ~LOMSimMgr(){}

    void cancelEvents( const vector<const Event *> &eventsToCancel ){
      negativeEvents.insert( negativeEvents.begin(),
			     eventsToCancel.begin(),
			     eventsToCancel.end() );
			    
    }

    unsigned int negativeCount(){
      return negativeEvents.size();
    }

    void checkNegativeEvents(){
      CPPUNIT_ASSERT( negativeEvents.size() == getDefaultEvents().size() );
      vector<const Event *>::iterator i = find( negativeEvents.begin(),
						negativeEvents.end(),
						getEvent3() );
      CPPUNIT_ASSERT( i != negativeEvents.end() );
      const Event *event4 = *i;
      CPPUNIT_ASSERT( event4 != 0 );
      negativeEvents.erase( i );
      const Event *event1 = negativeEvents[0];
      CPPUNIT_ASSERT( event1 != 0 );
      const Event *event2 = negativeEvents[1];
      CPPUNIT_ASSERT( event2 != 0 );
      const Event *event3 = negativeEvents[2];
      CPPUNIT_ASSERT( event3 != 0 );
      checkDefaultEvents( event1, event2, event3, event4 );
    }
  private:
    vector<const Event *> negativeEvents;
  };

  class LazyOutputManagerTestClass : public LazyOutputManager {
  public:
    LazyOutputManagerTestClass( TimeWarpSimulationManager *simMgr )
      : LazyOutputManager( simMgr ) {}
    ~LazyOutputManagerTestClass() {}
      
    vector<const Event*> getLazyCancelEvents() { return *(lazyQueues[0]); }

    OutputEvents &getOutputEventsFor( const ObjectID &objectId ) { 
      return OutputManagerImplementationBase::getOutputEventsFor(objectId);
    }

  };

public:
  TimeWarpSimulationManager *
  allocateTimeWarpSimulationManager( SimulationObject *initObject ){
    return new LOMSimMgr( initObject );
  }

  LazyOutputManagerTest();

  void setUp();
  void teardown();

  void testConstructor();
  void testInsert();
  void testRollback();
  void testFossilCollect();
  void testFossilCollectInt();
  void testOfcPurge();
  void testLazyCancel();

  unsigned int getNumElements( OutputManager *getSizeOf, const ObjectID &objectId );
  unsigned int getNumLazyCancelEvents( OutputManager *getSizeOf, const ObjectID &objectId );

protected:  

private:
  OutputManager *toTest;

};

#endif
