# Concurrent pins could lock one buffer record and return another

On ReactOS `4fbbefd614b`, `CcpGetAppropriateBcb` could allocate a provisional BCB, then find that another caller had already inserted a matching record. It acquired the provisional object's lock but incremented the pin count of the winner, discarded the provisional object and returned the winner. The user's later unpin therefore did not release the lock originally taken.

[`CcDuplicateBcb.patch`](Assets/History/CcDuplicateBcb.patch) acquired and returned the same BCB. It also released the losing allocation and kept ownership of the incoming view reference with the caller on failure. Changing just the lock expression would have missed those cleanup paths. The repair is now part of [`CcBcbOwnership.patch`](../FAT32VolumeFlush/CcBcbOwnership.patch).

The [host test](Assets/DuplicatePins.inc) publishes a competitor between the two lookups and observes references, locks and frees. Shared, exclusive and nonwaiting paths, allocation failure and a failed map are covered: **5/12 → 12/12**. There is no evidence that this race caused the installation failure.

Run the [host case](Assets/Run.cmd) from a native VS developer prompt. Pass `--stock` to exercise the pinned source.
