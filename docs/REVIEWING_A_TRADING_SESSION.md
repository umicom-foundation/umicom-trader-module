# Review a retained trading session

A session report lets you inspect the orders, account executions and positions
currently retained by Umicom Trader. An **execution** is a fill received for one
of your orders. It is separate from a public market trade in Time and Sales.

## Capture and inspect a report

1. Open the **Strategy Analysis** layout and find **Trade Performance**.
   Expand **Session review and CSV**. The same section is available in the
   shared **Executions** panel when that panel is present in a layout.
2. Read the account, environment and captured workspace revision at the top.
   A revision identifies the local workspace state; it is not a broker receipt.
3. Leave the instrument filter blank for the whole retained book, or enter an
   instrument ID, symbol, venue or currency, such as `NQ`, `CME` or `USD`.
   Matching ignores the case of English letters. Choose **Refresh report**.
4. Inspect the orders, executions and positions. Orders and positions follow
   their creation order. Executions follow arrival order, even if their event
   times arrived out of order. An event time is milliseconds since the Unix
   epoch, 1 January 1970 UTC; it is not a local clock reading.
5. Read **CONSISTENCY**. Framework replays retained fills using the same order
   and average-cost position rules as the trading workspace, then compares the
   result with the retained records. It does not alter or repair any record.

The existing panel details and refresh actions remain available above this
section. Changing the session filter does not change the watchlist, chart,
selected order or order ticket. Typing a filter does not immediately apply it.

## Understand the totals

The report groups gross realised profit and loss by currency. It does not add
GBP to USD or convert either to an account currency. Instrument multipliers are
already included by Framework's position accounting. A flat position can still
have realised profit or loss from earlier fills.

These figures exclude commissions, financing and tax. They are not a cash balance,
net trading return, tax report or unrealised valuation. A currency without matching
retained positions has no total row; an empty filter result does not prove a zero
external position.

## Copy exactly what you reviewed

6. Choose **Copy captured CSV**. Paste the table into your chosen editor or
   spreadsheet. The export contains the displayed report's revision, applied
   filter, retained and matching counts, source rows, discrepancies and available
   currency totals. Spreadsheet-like text prefixes are escaped by Framework.
7. If you edit the filter and copy before refreshing, the old displayed report
   is exported. This keeps the exported evidence consistent with what you read.
8. When the workspace changes, the report shows a message. Its contents stay
   fixed until **Refresh report**. Ordinary market updates and panel refreshes
   preserve both the report and your typed filter. Recreating or switching layouts
   can create a new report section with its default filter.

## Investigate a discrepancy

Consistency checks cover the whole retained book before filtering. Filtering
for one instrument cannot hide a disagreement elsewhere. Currency totals are
withheld when there is a discrepancy; the original source rows remain visible.

- A quantity, average price or realised P&L difference means the stored summary
  does not agree with the retained fills under the same arithmetic.
- A missing related record or duplicate identity means some retained evidence
  cannot be linked unambiguously.
- An instrument-definition difference can involve currency, multiplier, symbol,
  venue or expiry, even when the instrument ID matches.
- An account mismatch or an order from a different environment is shown explicitly.
  Changing the displayed environment does not relabel historical orders.

Export the evidence and investigate the source. Do not use the report to infer
that it is safe to transmit an order. It does not contact a broker, compare a
broker's positions, recover missing external history or authorize live trading.

The workspace currently retains bounded in-memory order, execution and position
catalogues. This report adds no financial-session persistence. Copy evidence
before ending a session if you need an external record. Saved chart annotations
and saved layouts are separate capabilities and do not imply saved order history.

If a refresh fails, its previous report stays displayed. Correct the filter or
inspect the error, then refresh again. Closing the workspace disables retained
report controls; it cannot leave a callback pointing into a destroyed trading book.
