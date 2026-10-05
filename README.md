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
