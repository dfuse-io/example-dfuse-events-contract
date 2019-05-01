## An example contract for dfuse Events

The purpose of this repository is to demonstrate a sample
EOS smart contract leveraging dfuse Events to control indexing
of those transactions within dfuse Search.

The general idea behind dfuse Events is to send a context-free
inline action `dfuseiohooks:event(key, urlEncodedFields)` acting
as a marker for dfuse Search to index the calling action (your
contract's action) based on the fields `urlEncodedFields`.

Let's break that down from the bottom, the inline context-free
action up to a search query to find something specific your
contract as done.

To start the example, let's use a fictive business use
case. Assume a smart contract that manages cards, each card
having a unique id and a kind (one of `club`, `diamond`, `spade`
and `heart`). This smart contract has a `move(from, to, card_id, card_kind)`
action that move a card between two accounts.

Now, you would like to search the blockchain to easily find all
transaction containing an action `<contract>:move` for the
various kind of cards.

First step is to modify a bit contract's `move` action so that it
sends an inline context-free action to the `dfuseiohooks:event`
action.

The inline context-free action parameters must be provided using
a strict pre-defined format that must be respected to be correctly
indexed.

The first parameter `key` is a string that must be the empty string
`""` or a currently valid dfuse Events unrestricted key that we
can provide to you to lift built-in indexing restrictions (max
5 fields, max of 64 characters per field's name, max 64 characters
per field's value).

**Note** Contact us directly to request a dfuse Events unrestricted key.

The second parameter `data` is string containing an url encoded
string representing a key/value pair list. For example, the string
`card_id=123&card_kind=club` represents the key/value pair list
`[{ key: card_id, value: 123 }, { key: card_kind, value: club }]`.

So, let's go ahead, from our contract's `passto` method, let's
instruct dfuse Search to index by `card_id` and by `card_kind`:

```
action(
    std::vector<permission_level>(),
    "dfuseiohooks"_n,
    "event"_n,
    std::make_tuple(string(""), "card_id=" + card_id + "&card_kind=" + card_kind)
).send_context_free();
```

This snippet of code create's an action `dfuseioshooks:event` (
`"dfuseiohooks"_n, "event"_n` arguments) with
no authorization (`std::vector<permission_level>()`, need to be empty
since it will be a context-free one) with the first parameter
being empty (the key) and the second one being an url encoded string
with first key/value pair being the `card_id:<card_id>` received
a second key/value pair being the `card_kind:<card_kind>` received.

When your transaction will be accepted by the blockchain, dfuse Search
will be notified, it will first check you `key` parameter to
ensures it the empty string or a valid key to decide of indexing
restrictions to use.

It will decode the second parameter of the `dfuseiohooks:event`
action, turn it into a list of key/value pair and will then
index the transaction specially, assigning each of the decomposed
field to the `event.<key>:<value>` SQE field.

Now that the action has been indexed, you can search for exactly
the transaction you are interested with.

- Search all `passto` action that had `card_kind` set to spade with `event.card_kind:spade parent.receiver:thecardgame`

- Search all `passto` action that dealing with `card_id` sets to `123` with `event.card_id:123 parent.receiver:thecardgame`

**Important** The `parent.receiver:<contract>` should always be used to
ensure that it was really **your** `<contract>` that sent the inline action.
You would not like that someone doing the same `card_id=123&card_kind=club`
fields indexing be included in your search result!

You can check the results getting out of a test account in eosq.app
directly: [event.card_kind:spade parent.receiver:maouehmaoueh](https://eosq.app/search?q=event.card_kind%253Aspade%2520parent.receiver%253Amaouehmaoueh)
