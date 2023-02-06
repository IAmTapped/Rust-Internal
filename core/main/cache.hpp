namespace bonecache {
	box_bounds get_bounds(BasePlayer* player, float expand = 0) {
		box_bounds ret = { FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN };

		for (auto j : valid_bones) {
			auto arr = player->model( )->boneTransforms( );
			if (arr) {
				auto transform = player->model( )->boneTransforms( )->get(j);
				if (transform) {
					Vector2 sc;
					auto world_pos = transform->position( );

					if (j == 48)
						world_pos.y += 0.2f;

					if (Camera::world_to_screen(world_pos, sc)) {
						Vector2 bone_screen = sc;

						if (bone_screen.x < ret.left)
							ret.left = bone_screen.x;
						if (bone_screen.x > ret.right)
							ret.right = bone_screen.x;
						if (bone_screen.y < ret.top)
							ret.top = bone_screen.y;
						if (bone_screen.y > ret.bottom)
							ret.bottom = bone_screen.y;
					}
				}
			}
		}

		if (ret.left == FLT_MAX)
			return box_bounds::null( );
		if (ret.right == FLT_MIN)
			return box_bounds::null( );
		if (ret.top == FLT_MAX)
			return box_bounds::null( );
		if (ret.bottom == FLT_MIN)
			return box_bounds::null( );

		ret.left -= expand;
		ret.right += expand;
		ret.top -= expand;
		ret.bottom += expand;

		return ret;
	};
	void cachePlayer(BasePlayer* player) {
		auto model = player->model( );
		if (model) {
			auto cache = new BoneCache( );

			cache->head = model->resolve(STATIC_CRC32("head"));
			cache->neck = model->resolve(STATIC_CRC32("neck"));
			cache->spine4 = model->resolve(STATIC_CRC32("spine4"));
			cache->spine1 = model->resolve(STATIC_CRC32("spine1"));
			cache->l_upperarm = model->resolve(STATIC_CRC32("l_upperarm"));
			cache->l_forearm = model->resolve(STATIC_CRC32("l_forearm"));
			cache->l_hand = model->resolve(STATIC_CRC32("l_hand"));
			cache->r_upperarm = model->resolve(STATIC_CRC32("r_upperarm"));
			cache->r_forearm = model->resolve(STATIC_CRC32("r_forearm"));
			cache->r_hand = model->resolve(STATIC_CRC32("r_hand"));
			cache->pelvis = model->resolve(STATIC_CRC32("pelvis"));
			cache->l_hip = model->resolve(STATIC_CRC32("l_hip"));
			cache->l_knee = model->resolve(STATIC_CRC32("l_knee"));
			cache->l_foot = model->resolve(STATIC_CRC32("l_foot"));
			cache->r_hip = model->resolve(STATIC_CRC32("r_hip"));
			cache->r_knee = model->resolve(STATIC_CRC32("r_knee"));
			cache->r_foot = model->resolve(STATIC_CRC32("r_foot"));
			cache->r_toe = model->resolve(STATIC_CRC32("r_toe"));
			cache->l_toe = model->resolve(STATIC_CRC32("l_toe"));

			if (player->userID( ) != LocalPlayer::Entity( )->userID( )) {
				box_bounds bo = get_bounds(player, 2.f);
				if (!bo.empty( ))
					cache->bounds = bo;

				Vector2 footPos;
				if (Camera::world_to_screen(cache->head->position, footPos))
					cache->dfc = footPos;

				Vector2 forwardd;
				if (Camera::world_to_screen(cache->head->position + (player->eyes( )->BodyForward( ) * 2), forwardd)) {
					cache->forward = forwardd;
				}

				auto mpv = player->find_mpv_bone( );
				Vector3 target;
				if (mpv != nullptr)
					target = mpv->position;
				else
					target = player->bones( )->head->position;
				
				cache->eye_rot = player->eyes( )->rotation( );
			}

			if (!map_contains_key(cachedBones, player->userID( )))
				cachedBones.insert(std::make_pair(player->userID( ), cache));
			else
				cachedBones[ player->userID( ) ] = cache;
		}
	}
}