# Umicom Trader Module

Thin C23 application composition for the Umicom Trader product.

The module intentionally contains **no duplicated trading, market-data, risk,
selection-routing or linked-workbench logic**. Those reusable capabilities live
in Umicom Framework. This first module phase consumes the Framework-owned
Trader/TMS workbench profile and exposes it as a product-specific composition.

Current composition:

- Trading group: `trading.red`
- Operations group: `operations.green`
- Watchlist
- Chart
- Order Entry
- Account Selector
- Risk
- Trade Blotter
- Context Inspector

Trader also starts Framework-owned learning, standard or focus presentation recipes with chart,
watchlist, depth, order, execution, portfolio, risk and strategy components.
The order helper prepares simulation state only; it cannot submit a live order.
See [the Trader application surface guide](docs/APPLICATION_SURFACE_GUIDE.md).
The [Trader runtime behavior and safety guide](docs/RUNTIME_BEHAVIOR_AND_TRADING_SAFETY.md)
explains full-rate market panels, guarded order commands, shared instrument
context and frequent workspace checkpoints.
The [workstation capability map](docs/TRADING_WORKSTATION_CAPABILITY_MAP.md)
separates available, foundational and planned workstation behaviour.

The graphical workstation uses the Framework appearance editor. A user can
choose dark, light, high-contrast, retro or neo styling, or create a custom
profile with their own fonts, text scale, control density and semantic
colours. The window remains resizable and layout editing continues to control
docking, floating, grouping and linked panel context independently of theme.

The executable, taskbar entry and installed desktop entry use the canonical
Umicom `<>` icon. The product name remains native text, so it stays clear and
accessible at different display scales.

Trader's public module headers now use the same complete file comment and
unique-guard audit as Framework and Studio, keeping the product SDK predictable
for future frontend and broker adapters.

Future Trader work can add product-specific presentation and broker adapters,
while canonical instruments, accounts, trades, risk, safeguards and context
routing remain Framework responsibilities.

The Windows suite installer exposes Trader as the optional **Trader**
component. Its graphical executable is registered with Umicom Desk, allowing
Trader and Studio or another installed product to run side by side.

## Order your workspace layouts

Trader's shared Layout Library includes **Move up** and **Move down** controls.
[Arrange your workspace layouts](docs/ORDERING_WORKSPACE_LAYOUTS.md) explains
how to reorder the list, retain the active workspace and save the order.

## Copy reports for review

Follow the applied order search through a copied report in [Copy an order report](docs/COPYING_ORDER_REPORTS.md).

- [Save, preview and restore drawings for one instrument](docs/SAVING_CHARTS.md).

## Review orders, executions and positions together

[Review a retained trading session](docs/REVIEWING_A_TRADING_SESSION.md) explains the native session report, consistency issues, instrument filters and captured CSV export.

## Mark and edit chart objects

[Draw ranges, zones and rays](docs/CHART_DRAWING_OBJECTS.md) explains the shared
chart tools, moving anchors, locks, duplication and explicit chart saves.


[Choose each drawing's colour, width and box fill](docs/DRAWING_APPEARANCE.md)
explains explicit appearance edits, tool defaults and saving the result.

- [Drawing undo and redo](docs/DRAWING_HISTORY.md)

## Review linked context values

Trader exposes Framework's reviewed context changes through
`umi_trader_workspace_context_review`, `umi_trader_workspace_context_apply`
and `umi_trader_workspace_clear_context` in
`umicom/trader/workspace_commands.h`. A context group is a named value that
related panels can share; for example, a host could use `trading.red` with the sample
value `EUR.USD`. The host must connect that name to its panel consumers.

1. Use a runtime initialised with this product's canonical experience. Prepare
   the requested changes with the review function; preparation changes no live state.
2. Display the copied Framework summary and rows. Keep the runtime and its
   workbench alive while the user reviews the proposed values. If the summary
   reports UI differences, show the captured UI value beside the cached value.
3. Apply only after acceptance, then destroy the review. If the workspace changed,
   prepare a fresh review. Cancelling only destroys the review.

This is a module API; native review screens are separate host work. Context edits
do not execute product commands or external operations. The shared guide at
`framework/docs/guides/REVIEWING_LINKED_CONTEXTS.html` in the Applications checkout
explains capacity, ownership, thread coordination and recovery in more detail.

[Read subscribed IBKR quotes](docs/READING_BROKER_QUOTES.html) explains the native workflow, retained settings and current limits.

Workspace recovery: [Recover chart and tool arrangements through the shared Layout Library](../../framework/docs/learning/restore-saved-workspaces.html). The guide explains the complete comparison, explicit confirmation and recovery limits.

Chart studies: see [the step-by-step guide](docs/CANDLE_STUDIES.html).


### Dated cash planning

Open **Cash planning** to maintain local dated inflows and outflows. Apply the
plan's settings, add entries with unique IDs, and inspect end-of-day balances and
the first projected buffer shortfall. You can disable or edit an entry, undo the
last stored change, save a new JSON file, review a loaded plan before applying
it, and export a CSV report with explicit currency and minor units.

Framework owns the complete workflow. These are manual assumptions; the planner
does not read account balances, broker buying power, submit payments or place
orders. Save explicitly before closing. See Framework's
`docs/learning/cash-planning.html` for a worked example and recovery steps.

The read-only Broker Connections window can write received selected-account values and positions to a new private CSV file. Each row carries completeness, stale-state and receipt-time information; exporting requests no new data and executes no order. See [Export broker observations](../../framework/docs/learning/export-broker-observations.html) for capture, import and recovery steps.

### Review received account positions

In Broker Connections, read an account and choose **Capture received positions
for review**. Filter the copied rows by symbol, asset type, currency, exchange,
contract ID or expiry, then inspect the exact provider quantity and cost text.
A current complete capture can copy its contract ID and exchange into the quote
inspector for separate review and subscription. It does not request positions
again, attest the Paper/Live environment, or place trades. Missing exchanges
must be obtained from TWS Contract Description. These controls require
Framework's GTK frontend. The full account display and CSV report stay unfiltered.


### Full-quantity intent and local liquidity watches

The Paper / Live broker connection window includes a full-quantity policy panel.
It describes FOK or AON fields and can watch the subscribed quote for five minutes
using your quantity, limit, optional size buffer and separate price/size freshness
limits. Reaching the displayed threshold does not submit an order or establish
that the contract and route support the instruction. Contract/route support stays
unconfirmed, and minimum quantity is not used as a substitute for full execution.
See the Framework guide at `docs/learning/full-quantity-orders.html` for the workflow
and its limits. Shared Framework services own the policy and watch; Trader uses
the existing connection window without a separate order engine.

### Inspect the instrument behind a quote

The IBKR connection window includes **Instrument description and route metadata**.
Subscribe to the intended contract and exchange, then select **Read current quote's contract description**.
Wait for the provider's end marker before treating the description as complete. The view shows identity,
reported order types and exchanges, size increments, minimum tick and market-rule identifiers.
These fields do not certify FOK or AON support. They do not change prices or submit orders.
See Framework's [instrument inspection guide](../../framework/docs/learning/ibkr-contract-inspection.html)
for the supported protocol range, response limits and recovery steps.

After the description completes, expand **Route price bands and limit review** to retrieve the selected
route's rule. The full-quantity limit is checked against the applicable price band's increment without
rounding or changing the entered price. Captured rules are historical metadata; reconnect for a fresh
capture. This review does not approve an order or establish FOK/AON support.

Recent execution capture is also available in the broker connection window. It retains distinct reports, ignores exact replays and compares ordinary stock fills by permanent order ID, contract and side. See the Framework [execution review guide](../../framework/docs/learning/review-broker-executions.html) for the workflow and its limits. This observation does not submit orders or infer final order status.

The execution view also displays matching commission reports and totals consistent exact fees in a single currency. Missing fees remain unavailable, and conflicting amounts retain their first and latest reported text for review. Consult the same execution guide for capture-window and correction limits.

## Find instruments and inspect live broker observations

The shared broker window includes symbol and company-name search, account or position P&L,
and direct or SMART market-depth subscriptions. Each view retains its own request identity;
cancelled or disconnected observations remain historical. Depth reset messages clear both sides
before new entries are applied.

Follow [Find instruments and read P&L and market depth](../../framework/docs/learning/broker-market-observations.html)
for the controls, broker prerequisites, exact-decimal limits and recovery steps. These observation
panels do not place orders, certify complete liquidity or guarantee full-quantity execution.

## Recover currently open broker orders

In the shared broker connection window, expand **Open orders and broker identity**
and choose client-only or all-visible-client recovery. The report retains broker
identities, contract/order summaries and cumulative status quantities. Duplicate
callbacks do not create additional fills, and disconnected or old observations
remain marked as historical.

Follow [Recover and review broker orders](../../framework/docs/learning/recover-broker-orders.html)
for connection settings, scope, timeout recovery and raw-field limits. This is a
point-in-time observation workflow; it does not submit, bind, modify or cancel orders.

## Review completed broker orders

The shared broker window includes **Completed orders and execution review**.
It retains the broker's final status, completion time, reported filled quantity
and permanent order ID separately from the open-order capture. Matching stock
executions can be compared with the reported filled quantity, while missing or
inconsistent commission reports remain unavailable.

Follow [Review completed orders and fills](../../framework/docs/learning/review-completed-orders.html)
for capture scope, freshness, account selection and recovery. A completed order
may have been cancelled after partial fills; this view does not imply that its
original quantity traded or enable order submission.

The same completed-order section can write a new local CSV report. Every row
carries the capture scope and completeness/freshness flags. The report owns
its bytes before the background write begins, and existing files are refused.


The shared broker connection window also includes finite historical candlestick captures.
Use the existing Contract ID and Exchange fields, open **Historical candlestick chart**,
and request an intraday range. The zoom and pan controls explore the captured bars without
another network request. This view is read-only and does not stream updates or place orders.
See the Framework [historical chart guide](../../framework/docs/learning/broker-historical-charts.html)
for supported intervals, request limits and recovery steps.

### Streaming broker charts

The shared broker workbench includes an explicit five-second bar subscription,
a rolling candlestick view with zoom and pan, and an owned CSV export. It keeps
up to 512 bars, reports discarded bars and time gaps, and clears stale charts.
Cancellation and broker corrections retain the original observations for review.
See Framework's docs/learning/broker-streaming-charts.html for the workflow,
market-data requirements and current protocol limits.

## Discover contracts with scanners and option chains

The shared broker connection window includes a market scanner and option-chain browser.
Start a scan with explicit market and filter settings, inspect its ranked contracts, and copy a selected
contract ID into the quote controls. Quotes require a separate subscription.

For options, request definitions for a resolved underlying, choose an expiry, strike and right,
then resolve that candidate with the broker. Only a completed, unambiguous result can supply the
option contract ID. Discovery does not place orders. Both panels can save a frozen CSV report with
their completeness and stale-state flags.

See [the discovery walkthrough](../../framework/docs/learning/broker-discovery.html) for the
controls, storage bounds and recovery steps.

The connection window also provides a searchable scanner parameter catalogue from TWS. It displays the broker's XML as read-only text, so you can inspect available scan codes, locations and generic filter names before entering a subscription. Read the catalogue once per connection; reconnect to refresh it.
