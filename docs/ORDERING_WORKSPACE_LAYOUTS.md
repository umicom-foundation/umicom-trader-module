# Arrange your Trader workspace layouts

You can put the layouts you use most near the top of Trader's layout selector,
such as your market view followed by research and strategy development.
Reordering changes presentation only. It does not submit or cancel orders,
change the selected instrument, switch trading environments or arm live trading.

## Move a layout

1. Apply or cancel any panel-layout edit.
2. Open **Layout Library** in the workstation header.
3. Clear the search box so the whole list is visible.
4. Select a layout, then click **Move up** or **Move down**.
5. Check the new order in the layout selector. The active layout stays open.
6. Choose **Save library** if you want to keep the complete list and order.

Selecting a library row chooses the target of an action; it does not open that
layout. Use **Open** when you actually want to switch workspaces. Moving a row
keeps its panels, names, identifiers, lock state and configuration together.

## Save and restore deliberately

The storage message distinguishes disk storage from a memory-only checkpoint.
A memory-only checkpoint does not survive exit. **Save layout** stores the
current arrangement; **Save library** stores the named list, its order and
active selection. Neither command saves a trading account or trading history.

To recover a saved list, select the confirmation beside **Restore library**
and then click **Restore library**. This replaces unsaved library changes.
Read errors retain the current list and explain that storage needs attention.

## Resolve unavailable controls

**Move up** is unavailable at the top, and **Move down** at the bottom. Both
are unavailable while search filters the list, a panel edit is open or another
library request is pending. Clear search or finish the pending operation.
If the library changed after your click, refresh it and retry after checking
the selected row. Do not interpret an unsuccessful save as a disk checkpoint.

Trader uses Framework's workspace owner and shared native controls. Layout
commands have no broker transport or execution responsibility. Normal trading
and connection controls retain their existing separate rules.

## Make your own copy of a layout

1. Finish or cancel a panel-layout edit, then open **Layout Library**.
2. Select an existing layout as your starting point.
3. Enter a useful **Layout name**, such as `My daily workspace`.
4. Leave **New layout ID (optional)** empty, then click **Duplicate**.
5. Trader opens the new copy. Its arrangement begins with the source layout.
6. Edit the copy's panels, apply and lock the arrangement, then choose
   **Save library** to retain the complete named list and active selection.

Framework chooses an unused internal ID automatically, checking all layouts,
including any hidden by search. You can still type a manual full ID when you
need one; keep the same application prefix and choose an unused identity.
The original layout stays in the library. Layout copying does not create orders, switch trading environments or arm live trading. Account data and trading history remain separate.

If another action changed the library after your click, refresh it, review
your selection and retry. A full library needs space before a copy can be
created. If a very long source ID leaves no room for a generated suffix, enter
a shorter manual ID with the same application prefix. Read the storage message
before exiting: a memory-only save does not survive restart.

## Review a saved list before replacing the current one

Choose **Preview saved** in Layout Library to read the saved names, order,
active selection and panel counts without restoring them. Follow
[Previewing saved layouts](PREVIEWING_SAVED_LAYOUTS.md) for a short walkthrough,
including how another window's later save affects Restore.

## Open a layout with the keyboard

1. Finish any active layout edit and open **Layout Library**.
2. Search if you want a shorter list, then move keyboard focus to a visible row.
3. Press **Enter** to open that row. You can also double-click it or select it
   and choose **Open**. Single clicks only select; they do not switch layouts.
4. If the list changed while the request was waiting, choose **Refresh**, check
   the visible selection, and try again. The application does not retry a stale
   request automatically.

Opening a layout changes the current panel arrangement. It does not save a
document, send an order or save the layout library. Use **Save library** when
you want to persist the list and its active selection.
