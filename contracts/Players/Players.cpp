#include "Players.hpp"

namespace Oasis {
    using namespace eosio;
    using namespace std;

    class Players: public contract {
        using contract::contract;

        public:
            Players(account_name self):contract(self) {
                // this is empty contstructor
            }
            //@abi action
            void add(
                const account_name account,
                string& username,
                uint64_t level
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
                playerIndex players(_self, _self);

                // verify the account does not exists yet.
                auto iterator = players.find(account);
                eosio_assert(
                    iterator == players.end(),
                    "Address for the account already exists"
                );

                /**
                 * we add the new player in the table
                 * The first argument is the plyer of the storage which will store the data
                 **/
                Players.emplace(account, [&](auto& player) {
                    player.account_name = account;
                    player.username = username;
                    player.level = 1;
                    player.health_points = 1000;
                    player.energy_points = 1000;
                });
            }

            //@abi action
            void update(
                const account_name account,
                uint64_t level,
                int64_t healthPoints,
                int64_t energyPoints
            ) {
                require_auth(account);

                playerIndex players(_self, _self);

                auto iterator = players.find(account);
                eosio_assert(
                    iterator != players.end(),
                    "address for the account is not found"
                );

                players.modify(iterator, account, [&](auto& player) {
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

            //@abi action
            void getplayer(const account_name account) {
                playerIndex players(_self, _self);

                auto iterator = players.find(account);
                eosio_assert(
                    iterator != players.end(),
                    "address for the account is not found"
                );

                auto currentPlayer = players.get(account);
                print(
                    "Username : ", currentPlayer.username.c_str(),
                    "Level : ", currentPlayer.level,
                    "Health: ", currentPlayer.health_points,
                    "Energy: ", currentPlayer.energy_points
                );
            }

        private:
            //@abi table player i64
            struct player {
                uint64_t account_name;
                string username;
                uint64_t level;
                uint64_t health_points = 1000;
                uint64_t energy_points = 1000;

                uint64_t primary_key() const { return account_name; }

                EOSLIB_SERIALIZE(player, (account_name)(username)(level)(health_points)(energy_points))
            };

            typedef multi_index<N(player), player> playerIndex;
    };

    EOSIO_ABI(Players, (add)(update)(getplayer))
}