// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_STREAM_ENDPOINT_I_H
#define ICE_STREAM_ENDPOINT_I_H

#include <Ice/ProtocolInstance.h>
#include <Ice/IPEndpointI.h>
#include <Ice/WSEndpoint.h>
#include <Ice/EndpointFactory.h>
#include <Ice/InstanceF.h>

#include <CoreFoundation/CFDictionary.h>
#include <CFNetwork/CFNetwork.h>

namespace Ice
{

class OutputStream;
class InputStream;

}

namespace IceObjC
{

class Instance : public IceInternal::ProtocolInstance
{
public:

    Instance(const Ice::CommunicatorPtr&, Ice::Short, const std::string&, bool);
    virtual ~Instance();

    const std::string& proxyHost() const
    {
        return _proxyHost;
    }

    int proxyPort() const
    {
        return _proxyPort;
    }

    void setupStreams(CFReadStreamRef, CFWriteStreamRef, bool, const std::string&) const;

    Instance* clone(const IceInternal::ProtocolInstancePtr&);

private:

    const bool _voip;
    const Ice::CommunicatorPtr _communicator;
    CFMutableDictionaryRef _proxySettings;
    std::string _proxyHost;
    int _proxyPort;
};
typedef IceUtil::Handle<Instance> InstancePtr;

class StreamAcceptor;
typedef IceUtil::Handle<StreamAcceptor> StreamAcceptorPtr;

class StreamEndpointI;
#ifdef ICE_CPP11_MAPPING // C++11 mapping
typedef ::std::shared_ptr<StreamEndpointI> StreamEndpointIPtr;
#else
typedef IceUtil::Handle<StreamEndpointI> StreamEndpointIPtr;
#endif

class StreamEndpointI : public IceInternal::IPEndpointI, public Ice::EnableSharedFromThis<StreamEndpointI>
{
public:

    StreamEndpointI(const InstancePtr&, const std::string&, Ice::Int, const IceInternal::Address&, Ice::Int,
                    const std::string&, bool);
    StreamEndpointI(const InstancePtr&);
    StreamEndpointI(const InstancePtr&, Ice::InputStream*);

    virtual Ice::EndpointInfoPtr getInfo() const;

    virtual Ice::Int timeout() const;
    virtual IceInternal::EndpointIPtr timeout(Ice::Int) const;
    virtual bool compress() const;
    virtual IceInternal::EndpointIPtr compress(bool) const;
    virtual bool datagram() const;
    virtual bool secure() const;

    virtual void connectors_async(Ice::EndpointSelectionType, const IceInternal::EndpointI_connectorsPtr&) const;
    virtual IceInternal::TransceiverPtr transceiver() const;
    virtual IceInternal::AcceptorPtr acceptor(const std::string&) const;
    virtual std::string options() const;

#ifdef ICE_CPP11_MAPPING
    virtual bool operator==(const Ice::Endpoint&) const;
    virtual bool operator<(const Ice::Endpoint&) const;
#else
    virtual bool operator==(const Ice::LocalObject&) const;
    virtual bool operator<(const Ice::LocalObject&) const;
#endif

    StreamEndpointIPtr endpoint(const StreamAcceptorPtr&) const;

    using IPEndpointI::connectionId;
    using Ice::EnableSharedFromThis<StreamEndpointI>::shared_from_this;

protected:

    virtual void streamWriteImpl(Ice::OutputStream*) const;
    virtual void hashInit(Ice::Int&) const;
    virtual bool checkOption(const std::string&, const std::string&, const std::string&);

    virtual IceInternal::ConnectorPtr createConnector(const IceInternal::Address&,
                                                      const IceInternal::NetworkProxyPtr&) const;
    virtual IceInternal::IPEndpointIPtr createEndpoint(const std::string&, int, const std::string&) const;

private:

    const InstancePtr _instance;

    //
    // All members are const, because endpoints are immutable.
    //
    const Ice::Int _timeout;
    const bool _compress;
};

class StreamEndpointFactory : public IceInternal::EndpointFactory
{
public:

    StreamEndpointFactory(const InstancePtr&);

    virtual ~StreamEndpointFactory();

    virtual Ice::Short type() const;
    virtual std::string protocol() const;
    virtual IceInternal::EndpointIPtr create(std::vector<std::string>&, bool) const;
    virtual IceInternal::EndpointIPtr read(Ice::InputStream*) const;
    virtual void destroy();

    virtual IceInternal::EndpointFactoryPtr clone(const IceInternal::ProtocolInstancePtr&,
                                                  const IceInternal::EndpointFactoryPtr&) const;

private:

    InstancePtr _instance;
};

}

#endif
