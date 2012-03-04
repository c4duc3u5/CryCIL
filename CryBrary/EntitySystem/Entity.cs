﻿using System.ComponentModel;

namespace CryEngine
{
    enum EntityMoveType
    {
        None = 0,
        Normal,
        Fly,
        Swim,
        ZeroG,

        Impulse,
        JumpInstant,
        JumpAccumulate
    }

    struct EntityMovementRequest
    {
        public EntityMoveType type;

        public Vec3 velocity;
    }

    /// <summary>
    /// </summary>
    public class Entity : StaticEntity
    {
        /// <summary>
        /// Initializes the entity, not recommended to set manually.
        /// </summary>
        /// <param name="entityId"></param>
        [EditorBrowsable(EditorBrowsableState.Never)]
        internal override bool InternalSpawn(uint entityId)
        {
            SpawnCommon(entityId);
            _CreateGameObjectForEntity(Id);
            OnSpawn();

			return IsEntityFlowNode();
        }
    }
}