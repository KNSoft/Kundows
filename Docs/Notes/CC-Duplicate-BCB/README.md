# Concurrent pins could lock one buffer record and return another

On ReactOS `4fbbefd614b`, `CcpGetAppropriateBcb` could allocate a provisional BCB, then find that another caller had already inserted a matching record. It acquired the provisional object's lock but incremented the pin count of the winner, discarded the provisional object and returned the winner. The user's later unpin therefore did not release the lock originally taken.

[`cc-duplicate-bcb.patch`](../FAT32-Volume-Flush/Assets/cc-duplicate-bcb.patch) acquired and returned the same BCB. It also released the losing allocation and kept ownership of the incoming view reference with the caller on failure. Changing just the lock expression would have missed those cleanup paths. The repair is now part of [`cc-bcb-ownership.patch`](../../../Upstream/ReactOS/Patches/Runtime/cc-bcb-ownership.patch).

The [host test](../../../Tests/ReactOS/Host/bcb.c) publishes a competitor between the two lookups and observes references, locks and frees. Shared, exclusive and nonwaiting paths, allocation failure and a failed map are covered: **5/12 → 12/12**. Recheck with `python Tests/ReactOS/Host/run.py Upstream/ReactOS/Source --suite bcb`. There is no evidence that this race caused the installation failure.
