#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>

using namespace eosio;
using namespace std;

class [[eosio::contract]] players: public contract {
    using contract::contract;

    public:
        [[eosio::action]]
        void hi( name user ) {
            require_auth(user);
            print("hi, ", user);
        }

        [[eosio::action]]
        void add(
            const name account,
            string& username
        ) {
            /**
             * We need to confirm that this will be invoked only by
             * someone who has the account
             **/
            require_auth(account);

            /**
             * We access the "player" table by creating an object
             * of type "playerIndex". As params we pass code &
             * scope - _self from the parent contract
             **/
            playerIndex players_db(_self, _self.value);

            // verify the account does not exists yet.
            auto iterator = players_db.find(account.value);
            eosio_assert(
                iterator == players_db.end(),
                "Address for the account already exists"
            );

            /**
             * we add the new player in the table
             * The first argument is the plyer of the storage which will store the data
             **/
            players_db.emplace(account, [&](auto& player) {
                player.account_name = account.value;
                player.username = username;
                player.level = 1;
                player.health_points = 1000;
                player.energy_points = 1000;
            });
        }

        [[eosio::action]]
        void update(
            const name account,
            uint64_t level,
            int64_t healthPoints,
            int64_t energyPoints
        ) {
            require_auth(account);

            playerIndex players_db(_self, _self.value);

            auto iterator = players_db.find(account.value);
            eosio_assert(
                iterator != players_db.end(),
                "address for the account is not found"
            );

            players_db.modify(iterator, account, [&](auto& player) {
                player.level = level;

                if ((player.health_points - healthPoints) < 0) {
                    player.health_points = 0;
                } else {
                    player.health_points -= healthPoints;
                }

                if ((player.energy_points - energyPoints) < 0) {
                    player.energy_points = 0;
                } else {
                    player.energy_points -= energyPoints;
                }
            });
        }

        [[eosio::action]]
        void getplayer(const name account) {
            playerIndex players_db(_self, _self.value);

            auto iterator = players_db.find(account.value);
            eosio_assert(
                iterator != players_db.end(),
                "address for the account is not found"
            );

            auto currentPlayer = players_db.get(account.value);
            print(
                "Username : ", currentPlayer.username.c_str(),
                "Level : ", currentPlayer.level,
                "Health: ", currentPlayer.health_points,
                "Energy: ", currentPlayer.energy_points
            );
        }

    private:
        struct [[eosio::table]] player {
            uint64_t account_name;
            string username;
            uint64_t level;
            uint64_t health_points = 1000;
            uint64_t energy_points = 1000;

            uint64_t primary_key() const { return account_name; }

            EOSLIB_SERIALIZE(player, (account_name)(username)(level)(health_points)(energy_points))
        };

        typedef multi_index<"player"_n, player> playerIndex;
};

EOSIO_DISPATCH( players, (add)(update)(getplayer) )
