#include <websocketpp/connection.hpp>
#include "include/bitshares.hpp"

Bitshares::Bitshares()
{

}

void Bitshares::Connect(std::string server, std::string wallet_path)
{
   graphene::wallet::wallet_data wallet_data;

   wallet_data.ws_server = server;
   wallet_data.ws_user = "";
   wallet_data.ws_password = "";
   websocket_connection  = websocket_client.connect( server );

   api_connection = std::make_shared<fc::rpc::websocket_api_connection>(*websocket_connection,
         GRAPHENE_MAX_NESTED_OBJECTS);

   remote_login_api = api_connection->get_remote_api< graphene::app::login_api >(1);
   database_api = api_connection->get_remote_api< graphene::app::database_api >(0);

   FC_ASSERT(remote_login_api->login( wallet_data.ws_user, wallet_data.ws_password ), "Failed to log in to API server");

   wallet_data.chain_id = database_api->get_chain_id();

   wallet_api_ptr = std::make_shared<graphene::wallet::wallet_api>(wallet_data, remote_login_api);

   wallet_api_ptr->set_wallet_filename(wallet_path);
   wallet_api_ptr->load_wallet_file();

   wallet_cli = std::make_shared<fc::rpc::cli>(GRAPHENE_MAX_NESTED_OBJECTS);

   for( auto& name_formatter : wallet_api_ptr->get_result_formatters() )
      wallet_cli->format_result( name_formatter.first, name_formatter.second );

   boost::signals2::scoped_connection closed_connection(websocket_connection->closed.connect([=]{
      cerr << "Server has disconnected us.\n";
      wallet_cli->stop();
   }));
}

void Bitshares::Disconnect()
{
   websocket_connection->close(0, "user disconnecting");

   wallet_cli->stop();

   api_connection = nullptr;
   wallet_api_ptr = nullptr;
}
