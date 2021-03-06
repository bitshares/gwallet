// This is a list of strings found in xrc files defined here for translation purposes
// File will change as more strings are generated, this header should remain
// wxrc -g resources/* -o i18n/xrc_strings.cpp

#line 32 "resources/about.xrc"
_("Software information");
#line 50 "resources/about.xrc"
_("Client version:");
#line 76 "resources/about.xrc"
_("Graphene revision:");
#line 102 "resources/about.xrc"
_("Revision age:");
#line 128 "resources/about.xrc"
_("FC revision:");
#line 154 "resources/about.xrc"
_("FC revision age:");
#line 180 "resources/about.xrc"
_("Compile date:");
#line 206 "resources/about.xrc"
_("Boost version:");
#line 232 "resources/about.xrc"
_("OpenSSL version:");
#line 258 "resources/about.xrc"
_("OS build:");
#line 25 "resources/borrowasset.xrc"
_("This is the first step in shorting an asset.  Call sell_asset() to complete the short.");
#line 26 "resources/borrowasset.xrc"
_("Borrow an asset or update the debt/collateral ratio for the loan.");
#line 49 "resources/borrowasset.xrc"
_("Borrower name");
#line 79 "resources/borrowasset.xrc"
_("The name or id of the account associated with the transaction.");
#line 115 "resources/borrowasset.xrc"
_("Amount to borrow");
#line 144 "resources/borrowasset.xrc"
_("The amount of the asset being borrowed.  Make this value negative to pay back debt.");
#line 154 "resources/borrowasset.xrc"
_("The symbol or id of the asset being borrowed.");
#line 189 "resources/borrowasset.xrc"
_("Amount of collateral");
#line 218 "resources/borrowasset.xrc"
_("The amount of the backing asset to add to your collateral position.  Make this negative to claim back some of your collateral.\nThe backing asset is defined in the bitasset_options for the asset being borrowed.");
#line 263 "resources/borrowasset.xrc"
_("Check to broadcast the transaction on the network.");
#line 264 "resources/borrowasset.xrc"
_("Broadcast transaction");
#line 273 "resources/borrowasset.xrc"
_("Show output in CLI mode");
#line 334 "resources/borrowasset.xrc"
_("&OK");
#line 25 "resources/cancelorder.xrc"
_("Cancel an existing order");
#line 48 "resources/cancelorder.xrc"
_("Order to cancel");
#line 78 "resources/cancelorder.xrc"
_(" Select order to be cancelled.");
#line 113 "resources/cancelorder.xrc"
_("Check to broadcast the transaction on the network.");
#line 114 "resources/cancelorder.xrc"
_("Broadcast transaction");
#line 123 "resources/cancelorder.xrc"
_("Show output in CLI mode");
#line 184 "resources/cancelorder.xrc"
_("&OK");
#line 28 "resources/cli.xrc"
_("Run command");
#line 36 "resources/cli.xrc"
_("Clear console");
#line 31 "resources/createaccountwithbrainkey.xrc"
_("Creates a new account and registers it on the blockchain.");
#line 60 "resources/createaccountwithbrainkey.xrc"
_("Brain key");
#line 89 "resources/createaccountwithbrainkey.xrc"
_("The brain key used for generating the account's private keys");
#line 99 "resources/createaccountwithbrainkey.xrc"
_("Get brain key");
#line 136 "resources/createaccountwithbrainkey.xrc"
_("Account name");
#line 165 "resources/createaccountwithbrainkey.xrc"
_("The name of the account, must be unique on the blockchain. \n\n Shorter names are more expensive to register; the rules are still in flux, but in general names of more than 8 characters with at least one digit will be cheap.");
#line 201 "resources/createaccountwithbrainkey.xrc"
_("Registrar account");
#line 230 "resources/createaccountwithbrainkey.xrc"
_("The account which will pay the fee to register the user");
#line 266 "resources/createaccountwithbrainkey.xrc"
_("Referrer account");
#line 295 "resources/createaccountwithbrainkey.xrc"
_("The account who is acting as a referrer, and may receive a portion of the user's transaction fees.  This can be the same as the registrar_account if there is no referrer.");
#line 330 "resources/createaccountwithbrainkey.xrc"
_("Check to broadcast the transaction on the network");
#line 331 "resources/createaccountwithbrainkey.xrc"
_("Broadcast transaction");
#line 340 "resources/createaccountwithbrainkey.xrc"
_("Show output in CLI mode");
#line 401 "resources/createaccountwithbrainkey.xrc"
_("&OK");
#line 24 "resources/createasset.xrc"
_("Creates a new user-issued or market-issued asset.");
#line 47 "resources/createasset.xrc"
_("Issuer");
#line 77 "resources/createasset.xrc"
_("The name of the account who will pay the fee and become the issuer of the new asset.  This can be updated later.");
#line 113 "resources/createasset.xrc"
_("Symbol");
#line 142 "resources/createasset.xrc"
_("The ticker symbol of the new asset");
#line 177 "resources/createasset.xrc"
_("Precision");
#line 207 "resources/createasset.xrc"
_("The number of digits of precision to the right of the decimal point, must be less than or equal to 12");
#line 244 "resources/createasset.xrc"
_("Common asset options");
#line 273 "resources/createasset.xrc"
_("Asset options required for all new assets.\n\nNote that core_exchange_rate technically needs to store the asset ID of this new asset. Since this ID is not known at the time this operation is created, create this price as though the new asset has instance ID 1, and the chain will overwrite it with the new asset's ID.");
#line 284 "resources/createasset.xrc"
_("Generate");
#line 321 "resources/createasset.xrc"
_("BitAsset options");
#line 351 "resources/createasset.xrc"
_("Options specific to BitAssets.\n\nThis may be null unless the market_issued flag is set in common flags.");
#line 361 "resources/createasset.xrc"
_("Generate");
#line 399 "resources/createasset.xrc"
_("Broadcast the transaction on the network");
#line 400 "resources/createasset.xrc"
_("Broadcast Transaction");
#line 409 "resources/createasset.xrc"
_("Show output in CLI mode");
#line 470 "resources/createasset.xrc"
_("&OK");
#line 24 "resources/createhtlc.xrc"
_("Create a hashed time lock contract");
#line 47 "resources/createhtlc.xrc"
_("Source");
#line 77 "resources/createhtlc.xrc"
_("The account that will reserve the funds (and pay the fee)");
#line 113 "resources/createhtlc.xrc"
_("Destination");
#line 142 "resources/createhtlc.xrc"
_("The account that will receive the funds if the preimage is presented");
#line 177 "resources/createhtlc.xrc"
_("Amount");
#line 186 "resources/createhtlc.xrc"
_("Asset");
#line 215 "resources/createhtlc.xrc"
_("The amount of the asset that is to be traded");
#line 227 "resources/createhtlc.xrc"
_("The asset that is to be traded");
#line 263 "resources/createhtlc.xrc"
_("Hash algorithm");
#line 272 "resources/createhtlc.xrc"
_("Preimage size in bytes");
#line 301 "resources/createhtlc.xrc"
_("The algorithm used to generate the hash from the preimage.");
#line 304 "resources/createhtlc.xrc"
_("RIPEMD160");
#line 305 "resources/createhtlc.xrc"
_("SHA1");
#line 306 "resources/createhtlc.xrc"
_("SHA256");
#line 316 "resources/createhtlc.xrc"
_("The size of the preimage in bytes");
#line 351 "resources/createhtlc.xrc"
_("Preimage hash");
#line 380 "resources/createhtlc.xrc"
_("The hash of the preimage");
#line 421 "resources/createhtlc.xrc"
_("Claim period");
#line 492 "resources/createhtlc.xrc"
_("Broadcast the transaction on the network");
#line 493 "resources/createhtlc.xrc"
_("Broadcast Transaction");
#line 502 "resources/createhtlc.xrc"
_("Show output in CLI mode");
#line 563 "resources/createhtlc.xrc"
_("&OK");
#line 24 "resources/extendhtlc.xrc"
_("Increase the timelock on an existing HTLC");
#line 47 "resources/extendhtlc.xrc"
_("Issuer");
#line 77 "resources/extendhtlc.xrc"
_("Who is performing this operation (and paying the fee)");
#line 113 "resources/extendhtlc.xrc"
_("HTLC ID");
#line 142 "resources/extendhtlc.xrc"
_("The account that will receive the funds if the preimage is presented");
#line 177 "resources/extendhtlc.xrc"
_("Minutes to add");
#line 207 "resources/extendhtlc.xrc"
_("How many minutes to add to the exiting timelock");
#line 250 "resources/extendhtlc.xrc"
_("Broadcast the transaction on the network");
#line 251 "resources/extendhtlc.xrc"
_("Broadcast Transaction");
#line 260 "resources/extendhtlc.xrc"
_("Show output in CLI mode");
#line 321 "resources/extendhtlc.xrc"
_("&OK");
#line 31 "resources/getaccounthistory.xrc"
_("Returns the most recent operations on the named account.");
#line 60 "resources/getaccounthistory.xrc"
_("Account name");
#line 122 "resources/getaccounthistory.xrc"
_("Limit");
#line 188 "resources/getaccounthistory.xrc"
_("Show output in CLI mode");
#line 249 "resources/getaccounthistory.xrc"
_("&OK");
#line 31 "resources/getaccount.xrc"
_(" Returns information about the given account.");
#line 60 "resources/getaccount.xrc"
_("Account name");
#line 117 "resources/getaccount.xrc"
_("Show output in CLI mode");
#line 172 "resources/getaccount.xrc"
_("&OK");
#line 31 "resources/getasset.xrc"
_("Returns information about the given asset.");
#line 60 "resources/getasset.xrc"
_("Asset name");
#line 117 "resources/getasset.xrc"
_("Show output in CLI mode");
#line 172 "resources/getasset.xrc"
_("&OK");
#line 31 "resources/getcommitteemember.xrc"
_("Returns information about the given committee member.");
#line 60 "resources/getcommitteemember.xrc"
_("Owner Account");
#line 117 "resources/getcommitteemember.xrc"
_("Show output in CLI mode");
#line 172 "resources/getcommitteemember.xrc"
_("&OK");
#line 31 "resources/gethtlc.xrc"
_("Returns information about the given HTLC object.");
#line 60 "resources/gethtlc.xrc"
_("HTLC ID");
#line 117 "resources/gethtlc.xrc"
_("Show output in CLI mode");
#line 172 "resources/gethtlc.xrc"
_("&OK");
#line 32 "resources/getorderbook.xrc"
_("Return the order book for a given market");
#line 61 "resources/getorderbook.xrc"
_("Market");
#line 141 "resources/getorderbook.xrc"
_("Limit");
#line 207 "resources/getorderbook.xrc"
_("Show output in CLI mode");
#line 268 "resources/getorderbook.xrc"
_("&OK");
#line 31 "resources/getwitness.xrc"
_("Returns information about the given witness.");
#line 60 "resources/getwitness.xrc"
_("Owner Account");
#line 117 "resources/getwitness.xrc"
_("Show output in CLI mode");
#line 172 "resources/getwitness.xrc"
_("&OK");
#line 7 "resources/importbalance_dialog.xrc"
_("Import Balance");
#line 33 "resources/importbalance_dialog.xrc"
_("Account Name");
#line 92 "resources/importbalance_dialog.xrc"
_("Private Key");
#line 189 "resources/importbalance_dialog.xrc"
_("&OK");
#line 196 "resources/importbalance_dialog.xrc"
_("&Cancel");
#line 7 "resources/importkey_dialog.xrc"
_("Import Key");
#line 33 "resources/importkey_dialog.xrc"
_("Account Name");
#line 92 "resources/importkey_dialog.xrc"
_("Private Key");
#line 189 "resources/importkey_dialog.xrc"
_("&OK");
#line 196 "resources/importkey_dialog.xrc"
_("&Cancel");
#line 32 "resources/info.xrc"
_("Blockchain information");
#line 50 "resources/info.xrc"
_("Head block number:");
#line 76 "resources/info.xrc"
_("Block age:");
#line 102 "resources/info.xrc"
_("Next maintenance time:");
#line 128 "resources/info.xrc"
_("Participation:");
#line 154 "resources/info.xrc"
_("Active witnesses:");
#line 163 "resources/info.xrc"
_("View");
#line 181 "resources/info.xrc"
_("Active committee:");
#line 190 "resources/info.xrc"
_("View");
#line 208 "resources/info.xrc"
_("Chain ID:");
#line 234 "resources/info.xrc"
_("Core asset:");
#line 260 "resources/info.xrc"
_("Blockchain name:");
#line 24 "resources/issueasset.xrc"
_(" Issue new shares of an asset.");
#line 53 "resources/issueasset.xrc"
_("To account");
#line 82 "resources/issueasset.xrc"
_("The name or id of the account to receive the new shares");
#line 117 "resources/issueasset.xrc"
_("Amount");
#line 146 "resources/issueasset.xrc"
_("The amount to issue, in nominal units");
#line 156 "resources/issueasset.xrc"
_("The ticker symbol of the asset to issue");
#line 191 "resources/issueasset.xrc"
_("Memo");
#line 221 "resources/issueasset.xrc"
_("A memo to include in the transaction, readable by the recipient");
#line 257 "resources/issueasset.xrc"
_("Broadcast the transaction on the network");
#line 258 "resources/issueasset.xrc"
_("Broadcast Transaction");
#line 267 "resources/issueasset.xrc"
_("Show output in CLI mode");
#line 328 "resources/issueasset.xrc"
_("&OK");
#line 6 "resources/main.xrc"
_("G-Wallet | Secure Bitshares Wallet");
#line 12 "resources/main.xrc"
_("File");
#line 14 "resources/main.xrc"
_("New");
#line 18 "resources/main.xrc"
_("Open");
#line 22 "resources/main.xrc"
_("Save");
#line 26 "resources/main.xrc"
_("Connection");
#line 31 "resources/main.xrc"
_("Change language");
#line 35 "resources/main.xrc"
_("Settings");
#line 39 "resources/main.xrc"
_("Quit");
#line 44 "resources/main.xrc"
_("Wallet");
#line 46 "resources/main.xrc"
_("Connect");
#line 50 "resources/main.xrc"
_("Disconnect");
#line 55 "resources/main.xrc"
_("Set password");
#line 59 "resources/main.xrc"
_("Lock");
#line 63 "resources/main.xrc"
_("Unlock");
#line 67 "resources/main.xrc"
_("Import key");
#line 71 "resources/main.xrc"
_("Import balance");
#line 76 "resources/main.xrc"
_("View");
#line 79 "resources/main.xrc"
_("Welcome");
#line 87 "resources/main.xrc"
_("Commands");
#line 94 "resources/main.xrc"
_("Wallet");
#line 101 "resources/main.xrc"
_("CLI");
#line 109 "resources/main.xrc"
_("Help");
#line 111 "resources/main.xrc"
_("About");
#line 124 "resources/main.xrc"
_("Connect");
#line 130 "resources/main.xrc"
_("Disconnect");
#line 136 "resources/main.xrc"
_("Lock");
#line 142 "resources/main.xrc"
_("Unlock");
#line 148 "resources/main.xrc"
_("Import Key");
#line 157 "resources/main.xrc"
_("My accounts");
#line 158 "resources/main.xrc"
_("MY ACCOUNTS");
#line 164 "resources/main.xrc"
_("My assets");
#line 165 "resources/main.xrc"
_("MY ASSETS");
#line 178 "resources/main.xrc"
_("My balance");
#line 217 "resources/main.xrc"
_("Welcome to G-Wallet");
#line 226 "resources/main.xrc"
_("Version 0.7.0");
#line 258 "resources/main.xrc"
_("Setup a new wallet by following the wizard. Please note you need a created account and private key.");
#line 295 "resources/main.xrc"
_("If you already have a wallet.json file somewhere please use this option to import it to G-Wallet.");
#line 332 "resources/main.xrc"
_("If your wallet is all set, connect to Bitshares.");
#line 24 "resources/redeemhtlc.xrc"
_("Update a hashed time lock contract");
#line 47 "resources/redeemhtlc.xrc"
_("Issuer");
#line 77 "resources/redeemhtlc.xrc"
_("Who is performing this operation (and paying the fee)");
#line 113 "resources/redeemhtlc.xrc"
_("HTLC ID");
#line 142 "resources/redeemhtlc.xrc"
_("The account that will receive the funds if the preimage is presented");
#line 177 "resources/redeemhtlc.xrc"
_("Preimage hash");
#line 206 "resources/redeemhtlc.xrc"
_("the preimage that should evaluate to the preimage_hash");
#line 247 "resources/redeemhtlc.xrc"
_("Broadcast the transaction on the network");
#line 248 "resources/redeemhtlc.xrc"
_("Broadcast Transaction");
#line 257 "resources/redeemhtlc.xrc"
_("Show output in CLI mode");
#line 318 "resources/redeemhtlc.xrc"
_("&OK");
#line 31 "resources/registeraccount.xrc"
_("This function is used to register an account for which you do not own the private keys. \n\n When acting as a registrar, an end user will generate their own private keys and send you the public keys. \n\n The registrar will use this function to register the account on behalf of the end user.");
#line 32 "resources/registeraccount.xrc"
_("Registers a third party's account on the blockckain.");
#line 67 "resources/registeraccount.xrc"
_("Account name");
#line 96 "resources/registeraccount.xrc"
_("The name of the account, must be unique on the blockchain. \n\n Shorter names are more expensive to register; the rules are still in flux, but in general names of more than 8 characters with at least one digit will be cheap.");
#line 132 "resources/registeraccount.xrc"
_("Owner key");
#line 161 "resources/registeraccount.xrc"
_("The owner key for the new account");
#line 197 "resources/registeraccount.xrc"
_("Active key");
#line 226 "resources/registeraccount.xrc"
_("The active key for the new account");
#line 262 "resources/registeraccount.xrc"
_("Registrar account");
#line 291 "resources/registeraccount.xrc"
_("The account which will pay the fee to register the user");
#line 327 "resources/registeraccount.xrc"
_("Referrer account");
#line 356 "resources/registeraccount.xrc"
_("The account who is acting as a referrer, and may receive a portion of the user's transaction fees.  This can be the same as the registrar_account if there is no referrer.");
#line 391 "resources/registeraccount.xrc"
_("Referrer percent");
#line 457 "resources/registeraccount.xrc"
_("Check to broadcast the transaction on the network");
#line 458 "resources/registeraccount.xrc"
_("Broadcast transaction");
#line 467 "resources/registeraccount.xrc"
_("Show output in CLI mode");
#line 528 "resources/registeraccount.xrc"
_("&OK");
#line 5 "resources/registration_wizard.xrc"
_("Setup wizard");
#line 16 "resources/registration_wizard.xrc"
_("This wizard will help you set up a new wallet.");
#line 25 "resources/registration_wizard.xrc"
_("Click Next to get started.");
#line 34 "resources/registration_wizard.xrc"
_("Click Cancel to exit the wizard.");
#line 48 "resources/registration_wizard.xrc"
_("First step to start using G-Wallet is to create a \nwallet file.");
#line 57 "resources/registration_wizard.xrc"
_("This file is used to store configuration options \nthat will be used to start the application next \ntime you use it.");
#line 78 "resources/registration_wizard.xrc"
_("Select wallet path and name:");
#line 119 "resources/registration_wizard.xrc"
_("Next step is to insert your API endpoint and \ncreate a wallet password.");
#line 128 "resources/registration_wizard.xrc"
_("Introduce your API endpoint:");
#line 168 "resources/registration_wizard.xrc"
_("Wallet password:");
#line 186 "resources/registration_wizard.xrc"
_("Repeat password:");
#line 215 "resources/registration_wizard.xrc"
_("If you already have an account in Bitshares \nand have a private key use the Import account option. ");
#line 224 "resources/registration_wizard.xrc"
_("To register a new account please use the \nCreate account option.");
#line 239 "resources/registration_wizard.xrc"
_("Registration options:");
#line 248 "resources/registration_wizard.xrc"
_("Import account - Need private key and \naccount name pair.");
#line 258 "resources/registration_wizard.xrc"
_("Create account - Will connect to a faucet \nservice.");
#line 278 "resources/registration_wizard.xrc"
_("Import your Bitshares account to G-Wallet.");
#line 293 "resources/registration_wizard.xrc"
_("Account:");
#line 310 "resources/registration_wizard.xrc"
_("Private key:");
#line 25 "resources/sellasset.xrc"
_("Buying and selling are the same operation on Graphene; if you want to buy BTS with USD, you should sell USD for BTS.\n\nThe blockchain will attempt to sell the symbol_to_sell for as much symbol_to_receive as possible, as long as the price is at least min_to_receive / amount_to_sell.\n       \nIn addition to the transaction fees, market fees will apply as specified by the issuer of both the selling asset and the receiving asset as a percentage of the amount exchanged.\n       \nIf either the selling asset or the receiving asset is whitelist restricted, the order will only be created if the seller is on the whitelist of the restricted asset type.\n       \n Market orders are matched in the order they are included in the block chain.");
#line 26 "resources/sellasset.xrc"
_(" Place a limit order attempting to sell one asset for another.");
#line 49 "resources/sellasset.xrc"
_("Seller account");
#line 79 "resources/sellasset.xrc"
_("The account providing the asset being sold, and which will receive the proceeds of the sale.");
#line 115 "resources/sellasset.xrc"
_("Amount to sell");
#line 144 "resources/sellasset.xrc"
_("The amount of the asset being sold to sell (in nominal units).");
#line 156 "resources/sellasset.xrc"
_("The name or id of the asset to sell.");
#line 192 "resources/sellasset.xrc"
_("Minimum amount to receive");
#line 221 "resources/sellasset.xrc"
_("The minimum amount you are willing to receive in return for selling the entire amount_to_sell.");
#line 232 "resources/sellasset.xrc"
_("The name or id of the asset you wish to receive.");
#line 267 "resources/sellasset.xrc"
_("Timeout");
#line 297 "resources/sellasset.xrc"
_("If the order does not fill immediately, this is the length of  time the order will remain on the order books before it is cancelled and the un-spent funds are returned to the seller's account.");
#line 306 "resources/sellasset.xrc"
_("If the order does not fill immediately, this is the length of  time the order will remain on the order books before it is cancelled and the un-spent funds are returned to the seller's account");
#line 340 "resources/sellasset.xrc"
_("If checked, the order will only be included in the blockchain if it is filled immediately; if not checked, an open order will be left on the books to fill any amount that cannot be filled immediately.");
#line 341 "resources/sellasset.xrc"
_("Fill or Kill");
#line 370 "resources/sellasset.xrc"
_("Check to broadcast the transaction on the network");
#line 371 "resources/sellasset.xrc"
_("Broadcast transaction");
#line 380 "resources/sellasset.xrc"
_("Show output in CLI mode");
#line 441 "resources/sellasset.xrc"
_("&OK");
#line 25 "resources/setproxy.xrc"
_(" If a user does not wish to take an active part in voting, they can choose to allow another account to vote their stake.\n\nSetting a vote proxy does not remove your previous votes from the blockchain, they remain there but are ignored.  If you later null out your vote proxy, your previous votes will take effect again.\n\n This setting can be changed at any time.");
#line 26 "resources/setproxy.xrc"
_("Set the voting proxy for an account.");
#line 49 "resources/setproxy.xrc"
_("Account to modify");
#line 79 "resources/setproxy.xrc"
_("The name or id of the account to update.");
#line 115 "resources/setproxy.xrc"
_("Voting account");
#line 144 "resources/setproxy.xrc"
_("The name or id of an account authorized to vote account_to_modify's shares, or null to vote your own shares.");
#line 179 "resources/setproxy.xrc"
_("Check if you wish to broadcast the transaction to the network.");
#line 180 "resources/setproxy.xrc"
_("Broadcast transaction");
#line 189 "resources/setproxy.xrc"
_("Show output in CLI mode");
#line 250 "resources/setproxy.xrc"
_("&OK");
#line 5 "resources/settings_dialog.xrc"
_("Settings");
#line 14 "resources/settings_dialog.xrc"
_("Change program settings. Use with caution.");
#line 36 "resources/settings_dialog.xrc"
_("Internal config");
#line 48 "resources/settings_dialog.xrc"
_("Wallet Path:");
#line 74 "resources/settings_dialog.xrc"
_("Server:");
#line 100 "resources/settings_dialog.xrc"
_("Language:");
#line 133 "resources/settings_dialog.xrc"
_("Are accounts imported to this wallet?");
#line 150 "resources/settings_dialog.xrc"
_("Clear everything");
#line 162 "resources/settings_dialog.xrc"
_("Save");
#line 173 "resources/settings_dialog.xrc"
_("Cancel");
#line 25 "resources/transfer.xrc"
_("Transfer asset to another bitshares account");
#line 48 "resources/transfer.xrc"
_("From");
#line 78 "resources/transfer.xrc"
_("The name or id of the account sending the funds");
#line 114 "resources/transfer.xrc"
_("To");
#line 177 "resources/transfer.xrc"
_("Amount");
#line 206 "resources/transfer.xrc"
_("The amount to send (in nominal units -- to send half of a BTS, specify 0.5)");
#line 218 "resources/transfer.xrc"
_("The symbol or id of the asset to send");
#line 254 "resources/transfer.xrc"
_("Memo");
#line 285 "resources/transfer.xrc"
_("A memo to attach to the transaction.  The memo will be encrypted in the transaction and readable for the receiver.  There is no length limit other than the limit imposed by maximum transaction size, but transaction increase with transaction size");
#line 321 "resources/transfer.xrc"
_("Broadcast the transaction on the network");
#line 322 "resources/transfer.xrc"
_("Broadcast Transaction");
#line 331 "resources/transfer.xrc"
_("Show output in CLI mode");
#line 392 "resources/transfer.xrc"
_("&OK");
#line 31 "resources/upgradeaccount.xrc"
_("This makes the account holder a 'lifetime member'.");
#line 32 "resources/upgradeaccount.xrc"
_("Upgrades an account to prime status.");
#line 61 "resources/upgradeaccount.xrc"
_("The name of the account to upgrade");
#line 91 "resources/upgradeaccount.xrc"
_("Broadcast the transaction on the network");
#line 92 "resources/upgradeaccount.xrc"
_("Broadcast Transaction");
#line 101 "resources/upgradeaccount.xrc"
_("Check to broadcast the transaction on the network");
#line 102 "resources/upgradeaccount.xrc"
_("Show output in CLI mode");
#line 157 "resources/upgradeaccount.xrc"
_("&OK");
#line 15 "resources/wallet.xrc"
_("Please unlock your wallet");
