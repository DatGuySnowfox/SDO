# What the game actually does when it dresses a character (UE 5.6)

Decoded 2026-09-25 from the live 5.6 build. Supersedes the 5.3-era notes, which
referenced `EquipClothingToMesh` - a function the live 5.6 class does not have at
all. The dumper rejected it outright: "EquipClothingToMesh function not found".

## MC_AttachClothing is the real entry point

    MC_AttachClothing(USkinnedMeshComponent* Clothing, USkinnedAsset* Mesh,
                      FBodyPartSettings Parts, bool IsPlayerMale?,
                      FName Body Part, bool UpdateAllBodyParts?)

Its own body is a thunk: it copies all six parameters into the persistent frame
and jumps to ubergraph offset 140324 (0x22424). The real body is two steps:

    [0x22424]  BodyPartVisibility( Parts, IsPlayerMale?, BodyPart, UpdateAllBodyParts? )
    [0x22456]  Clothing->SetSkinnedAssetAndUpdate( Mesh, false )

Identification is structural rather than by name, because the pipeline's FName
resolve step returned "0 names resolved" on this run: the first call takes frame
slots 3/4/5/6, which is exactly BodyPartVisibility's four-argument signature, and
the second is a two-argument call on the clothing component itself passing the
mesh and False, which is SetSkinnedAssetAndUpdate's shape. Worth confirming by
resolving ci=1696234 and ci=137933 before relying on it.

Note what is NOT in there: no SetVisibility, no SetHiddenInGame, and no write to
ClothingTorsoEquipped? or its siblings. Whatever governs visibility happens
inside BodyPartVisibility, which is consistent with those flags being replication
state rather than the mechanism - and with setting them by hand having made
things strictly worse.

## What this project does instead

equip_clothing_to_mesh performs step two and a hand-rolled approximation of step
one. BodyPartVisibility is 2716 bytes of decoded bytecode that repeatedly calls
one native function on component after component, passing struct members out of
the FBodyPartSettings it was handed. Our version clears four body parts from a
table of pairs that was assembled by watching a dressed local player.

Every input BodyPartVisibility wants is already in hand at that call site:

    Parts               FClothingSettings + 0x18 (FBodyPartSettings), already read
    IsPlayerMale?       already read by name
    UpdateAllBodyParts? FClothingSettings + 0x10
    Body Part           FName, source not yet identified

## Next step

Call BodyPartVisibility directly with those arguments instead of the hand-rolled
clear, keeping the existing SetSkinnedAssetAndUpdate as step two. Prefer it over
calling MC_AttachClothing, which is a multicast RPC and may behave differently
when invoked on a proxy.

Resolve the "Body Part" FName first. UpdateBodyParts(FName Name) takes the same
kind of value, and ComponentDriftCtx::bodyPartCi already carries per-part FName
comparison indices for Torso/Legs/Feet, so the vocabulary is probably already
known to this codebase.
