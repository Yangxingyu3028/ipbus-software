#include "uhal/ClientImplementation.hpp"

namespace uhal
{
	ControlHubClient::ControlHubClient ( const std::string& aId , const URI& aUri )
		: ClientInterface ( aId , aUri ),
		mPackingProtocolPtr ( NULL ),				
		mTransportProtocolPtr ( NULL )
	{
		std::string lHostId( mUri.mHostname + ":" + mUri.mPort );
	
		tMap::iterator lIt = ControlHubClient::mMapNameAndPortToCHH.find( lHostId );
		
		if( lIt == ControlHubClient::mMapNameAndPortToCHH.end() ){
			pantheios::log_INFORMATIONAL( "Creating ControlHubClient Map entry for hostname " , mUri.mHostname , ", port " , mUri.mPort  );
			mPackingProtocolPtr = new tPackingProtocol( mMaxPacketLength );
			mTransportProtocolPtr = new tTransportProtocol( mUri.mHostname , //hostname
															mUri.mPort , //port number
															*mPackingProtocolPtr, //reference to a PackingProtocol object which implements a function to handle the BOOST::ASIO callback
															mTimeoutPeriod //the timeout period for the TCP transactions in seconds
															);
															
			ControlHubClient::mMapNameAndPortToCHH.insert( 
														std::make_pair( 
															lHostId,
															std::make_pair( 
																mPackingProtocolPtr,
																mTransportProtocolPtr
															)
														)
													);																				
		}else{
			pantheios::log_INFORMATIONAL( "Found existing entry in ControlHubClient Map entry for hostname " , mUri.mHostname , ", port " , mUri.mPort  );
			mPackingProtocolPtr = lIt->second.first;
			mTransportProtocolPtr = lIt->second.second;
		}
	
		mTargetId = boost::lexical_cast< uint32_t >( mUri.mPath );
		pantheios::log_INFORMATIONAL( "Converted uri path \"" , mUri.mPath , "\" to device identifier " , pantheios::integer( mTargetId  , pantheios::fmt::fullHex | 10 ) );
	
	};
	
	ControlHubClient::~ControlHubClient (){
		for ( tMap::iterator lIt = ControlHubClient::mMapNameAndPortToCHH.begin();
								lIt != ControlHubClient::mMapNameAndPortToCHH.end();
								++lIt ){
			if( lIt->second.first ){
				delete lIt->second.first;
				lIt->second.first = NULL;
			}

			if( lIt->second.second ){
				delete lIt->second.second;
				lIt->second.second = NULL;
			}								
		}
	}
	
	
	ControlHubClient::tMap ControlHubClient::mMapNameAndPortToCHH = ControlHubClient::tMap();
}



