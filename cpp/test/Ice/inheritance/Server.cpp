// **********************************************************************
//
// Copyright (c) 2001
// MutableRealms, Inc.
// Huntsville, AL, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <Ice/Ice.h>
#include <TestI.h>

using namespace std;

void
usage(const char* n)
{
    cerr << "Usage: " << n << " [--pid]\n";
}

int
run(int argc, char* argv[], Ice::CommunicatorPtr communicator)
{
    bool pid = false;
    for (int i = 1; i < argc; ++i)
    {
	if(strcmp(argv[i], "--pid") == 0)
	{
	    pid = true;
	}
	else
	{
	    cerr << argv[0] << ": unknown option `" << argv[i] << "'" << endl;
	    usage(argv[0]);
	    return EXIT_FAILURE;
	}
    }

    string endpts("tcp -p 12345 -t 2000");
    Ice::ObjectAdapterPtr adapter = communicator -> createObjectAdapterWithEndpoints("testadapter", endpts);
    Ice::ObjectPtr object = new InitialI(adapter);
    adapter->add(object, "initial");
    adapter->activate();
    if (pid)
    {
	cout << getpid() << endl;
    }
    communicator->waitForShutdown();
    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[])
{
    int status;
    Ice::CommunicatorPtr communicator;

    try
    {
	communicator = Ice::initialize(argc, argv);
	status = run(argc, argv, communicator);
    }
    catch(const Ice::LocalException& ex)
    {
	cerr << ex << endl;
	status = EXIT_FAILURE;
    }

    if (communicator)
    {
	try
	{
	    communicator->destroy();
	}
	catch(const Ice::LocalException& ex)
	{
	    cerr << ex << endl;
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
