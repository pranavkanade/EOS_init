# EOS contracts

## Notes:
- Smart contracts communicate with each other in the form of actions and shared memory database access.
- A contract can read the state of another contract's database as long as it is included within the read scope of the transactions with an async vibe.
- This can be achieved by using one of two communication modes - **inline** and **deferred**. One can think of em' as *sync* and *async*.

> **Inline** - Inline is a guaranteed to execute with current transaction or unwind; no notification will be communicated regardless of success or failure. Inline operates with the same scopes and authorities the original transaction had.

> **Deferred** - Defer will get scheduled later at producer's discretion; it's possible to communicate the result of of the communication or can simply timeout. Deferred can reach out to different scopes and carry the authority of the contract taht sends em.

- We've put actions and constructors in *public* scope.
- Each action represents a single operation inside the smart contract.
- `//@abi action` is the indication to **eosiocpp** compiler.

- Each action operates within its own environment know as action context.
- Context provides :
    - working memory: Here action maintains working state.
- Before processing an action EOSIO sets up a clean working memory for the action.
- The only *way to pass state among action* is to persist it to and retrive it from the **EOSIO databse**
- This can be achieved with **multi-index**. It allows us to read/modify persistent state in the **EOSIO database**.
- One can define a table object with primary key. (take a look at Players.cpp)
- Once the object is ready, we need to typedef our *multi-index* as follows:
```cpp
typedef multi_index<N(table_name), object_template_to_use> multi_index_name;
```