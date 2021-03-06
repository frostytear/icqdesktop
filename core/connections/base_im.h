#ifndef __BASE_IM_H_
#define __BASE_IM_H_

#pragma once

#include "../../common.shared/themes_constants.h"

namespace voip_manager {
    class VoipManager;
    struct VoipProtoMsg;
    struct WindowParams;
    struct VoipProxySettings;
}

namespace core
{
    class login_info;
    class im_login_id;
    class phone_info;
    class search_params;
    enum class file_sharing_function;
    enum class message_type;
    enum class sticker_size;
    enum class profile_state;
    enum class typing_status;
    class auto_callback;

    namespace wim {
        class wim_packet;
        struct wim_packet_params;
    }

    namespace archive
    {
        class message_header;
        class quote;
        typedef std::list<int64_t>		msgids_list;
        typedef std::vector<quote>      quotes_vec;
    }

    namespace stickers
    {
        class face;
        class download_task;
    }

    namespace themes
    {
        class face;
        class theme;
    }

    class base_im
    {
        std::shared_ptr<voip_manager::VoipManager> voip_manager_;
        int32_t id_;

        // stickers
        // use it only this from thread
        std::shared_ptr<stickers::face>			stickers_;

        std::shared_ptr<themes::face>			themes_;

    protected:

        std::wstring get_contactlist_file_name();
        std::wstring get_my_info_file_name();
        std::wstring get_active_dilaogs_file_name();
        std::wstring get_favorites_file_name();
        std::wstring get_im_data_path();
        std::wstring get_file_name_by_url(const std::string& _url);
        std::wstring get_stickers_path();
        std::wstring get_themes_path();
        std::wstring get_im_downloads_path(const std::string &alt);
        std::wstring get_content_cache_path();

        virtual std::string _get_protocol_uid() = 0;
        void set_id(int32_t _id);
        int32_t get_id() const;

        std::shared_ptr<stickers::face> get_stickers();
        std::shared_ptr<themes::face> get_themes();

    public:
        base_im(const im_login_id& _login, std::shared_ptr<voip_manager::VoipManager> _voip_manager);
        virtual ~base_im();

        virtual void connect() = 0;
        virtual std::wstring get_im_path() const = 0;

        // login functions
        virtual void login(int64_t _seq, const login_info& _info) = 0;
        virtual void login_normalize_phone(int64_t _seq, const std::string& _country, const std::string& _raw_phone, const std::string& _locale, bool _is_login) = 0;
        virtual void login_get_sms_code(int64_t _seq, const phone_info& _info, bool _is_login) = 0;
        virtual void login_by_phone(int64_t _seq, const phone_info& _info) = 0;

        virtual void start_attach_uin(int64_t _seq, const login_info& _info, const wim::wim_packet_params& _from_params) = 0;
        virtual void start_attach_phone(int64_t _seq, const phone_info& _info) = 0;

        virtual std::string get_login() = 0;
        virtual void logout(std::function<void()> _on_result) = 0;

        virtual const wim::wim_packet_params make_wim_params() = 0;
        virtual const wim::wim_packet_params make_wim_params_general(bool _is_current_auth_params) = 0;

        virtual void erase_auth_data() = 0; // when logout
        virtual void start_session(bool _is_ping = false) = 0;
        virtual void handle_net_error(int32_t err) = 0;

        virtual void phoneinfo(int64_t _seq, const std::string &phone, const std::string &gui_locale) = 0;

        // messaging functions
        virtual void send_message_to_contact(
            const int64_t _seq,
            const std::string& _contact,
            const std::string& _message,
            const core::message_type _type,
            const std::string& _internal_id,
            const core::archive::quotes_vec& _quotes) = 0;

        virtual void send_message_typing(const int64_t _seq, const std::string& _contact, const core::typing_status& _status) = 0;

        // feedback
        virtual void send_feedback(const int64_t, const std::string &url, const std::map<std::string, std::string> &fields, const std::vector<std::string> &files) = 0;

        // state
        virtual void set_state(const int64_t, const core::profile_state) = 0;

        // group chat
        virtual void remove_members(int64_t _seq, const std::string& _aimid, const std::string& _m_chat_members) = 0;
        virtual void add_members(int64_t _seq, const std::string& _aimid, const std::string& _m_chat_members_to_add) = 0;
        virtual void add_chat(int64_t _seq, const std::string& _m_chat_name, const std::vector<std::string>& _m_chat_members) = 0;
        virtual void modify_chat(int64_t _seq, const std::string& _aimid, const std::string& _m_chat_name) = 0;

        // avatar function
        virtual void get_contact_avatar(int64_t _seq, const std::string& _contact, int32_t _avatar_size, bool _force) = 0;
        virtual void show_contact_avatar(int64_t _seq, const std::string& _contact, int32_t _avatar_size) = 0;

        // history functions
        virtual void get_archive_images(int64_t _seq_, const std::string& _contact, int64_t _from, int64_t _count) = 0;
        virtual void repair_archive_images(int64_t _seq_, const std::string& _contact) = 0;
        virtual void get_archive_messages(int64_t _seq_, const std::string& _contact, int64_t _from, int64_t _count) = 0;
        virtual void get_archive_index(int64_t _seq_, const std::string& _contact, int64_t _from, int64_t _count) = 0;
        virtual void get_archive_messages_buddies(int64_t _seq_, const std::string& _contact, std::shared_ptr<archive::msgids_list> _ids) = 0;
        virtual void set_last_read(const std::string& _contact, int64_t _message) = 0;
        virtual void hide_dlg_state(const std::string& _contact) = 0;
        virtual void delete_archive_messages(const int64_t _seq, const std::string &_contact_aimid, const std::vector<int64_t> &_ids, const bool _for_all) = 0;
        virtual void delete_archive_messages_from(const int64_t _seq, const std::string &_contact_aimid, const int64_t _from_id) = 0;

        virtual void add_opened_dialog(const std::string& _contact) = 0;
        virtual void remove_opened_dialog(const std::string& _contact) = 0;
        virtual void set_first_message(const std::string& _contact, int64_t _message) = 0;

        virtual void get_stickers_meta(int64_t _seq, const std::string& _size) = 0;
        virtual void get_sticker(int64_t _seq, int32_t _set_id, int32_t _sticker_id, core::sticker_size _size) = 0;
        virtual void get_chat_home(int64_t _seq, const std::string& _tag) = 0;
        virtual void get_chat_info(int64_t _seq, const std::string& _aimid, const std::string& _stamp, int32_t _limit) = 0;
        virtual void get_chat_blocked(int64_t _seq, const std::string& _aimid) = 0;
        virtual void get_chat_pending(int64_t _seq, const std::string& _aimid) = 0;
        virtual void get_themes_meta(int64_t _seq, const ThemesScale _themes_scale) = 0;
        virtual void resolve_pending(int64_t _seq, const std::string& _aimid, const std::vector<std::string>& _contact, bool _approve) = 0;

        virtual void mod_chat_name(int64_t _seq, const std::string& _aimid, const std::string& _name) = 0;
        virtual void mod_chat_about(int64_t _seq, const std::string& _aimid, const std::string& _about) = 0;
        virtual void mod_chat_public(int64_t _seq, const std::string& _aimid, bool _public) = 0;
        virtual void mod_chat_join(int64_t _seq, const std::string& _aimid, bool _approved) = 0;
        virtual void block_chat_member(int64_t _seq, const std::string& _aimid, const std::string& _contact, bool _block) = 0;
        virtual void set_chat_member_role(int64_t _seq, const std::string& _aimid, const std::string& _contact, const std::string& _role) = 0;

        // search functions
        virtual void search(std::vector<std::string> searchPatterns) = 0;

        // cl
        virtual std::string get_contact_friendly_name(const std::string& contact_login) = 0;
        virtual void hide_chat(const std::string& _contact) = 0;
        virtual void mute_chat(const std::string& _contact, bool _mute) = 0;
        virtual void add_contact(int64_t _seq, const std::string& _aimid, const std::string& _group, const std::string& _auth_message) = 0;
        virtual void remove_contact(int64_t _seq, const std::string& _aimid) = 0;
        virtual void rename_contact(int64_t _seq, const std::string& _aimid, const std::string& _friendly) = 0;
        virtual void spam_contact(int64_t _seq, const std::string& _aimid) = 0;
        virtual void ignore_contact(int64_t _seq, const std::string& _aimid, bool ignore) = 0;
        virtual void get_ignore_list(int64_t _seq) = 0;
        virtual void favorite(const std::string& _contact) = 0;
        virtual void unfavorite(const std::string& _contact) = 0;

        // voip
        //virtual void on_peer_list_updated(const std::vector<std::string>& peers) = 0;
        virtual void on_voip_call_request_calls();
        virtual void on_voip_call_set_proxy(const voip_manager::VoipProxySettings& proxySettings);
        virtual void on_voip_call_start(std::string contact, bool video, bool attach);
        virtual void on_voip_add_window(voip_manager::WindowParams& windowParams);
        virtual void on_voip_remove_window(void* hwnd);
        virtual void on_voip_call_end(std::string contact, bool busy);
        virtual void on_voip_call_accept(std::string contact, bool video);
        virtual void on_voip_call_stop();
        virtual void on_voip_proto_msg(bool allocate, const char* data, unsigned len, std::shared_ptr<auto_callback> _on_complete);
        virtual void on_voip_proto_ack(const voip_manager::VoipProtoMsg& msg, bool success);
        virtual void on_voip_update();
        virtual void on_voip_reset();

        virtual bool on_voip_avatar_actual_for_voip(const std::string& contact, unsigned avatar_size);
        virtual void on_voip_user_update_avatar(const std::string& contact, const unsigned char* data, unsigned size, unsigned avatar_h, unsigned avatar_w);
        virtual void on_voip_user_update_avatar_no_video(const std::string& contact, const unsigned char* data, unsigned size, unsigned h, unsigned w);
        virtual void on_voip_user_update_avatar_camera_off(const std::string& contact, const unsigned char* data, unsigned size, unsigned h, unsigned w);
        virtual void on_voip_user_update_avatar_no_camera(const std::string& contact, const unsigned char* data, unsigned size, unsigned h, unsigned w);
        virtual void on_voip_user_update_avatar_text(const std::string& contact, const unsigned char* data, unsigned size, unsigned h, unsigned w);
        virtual void on_voip_user_update_avatar_text_header(const std::string& contact, const unsigned char* data, unsigned size, unsigned h, unsigned w);
        virtual void on_voip_window_update_background(void* hwnd, const unsigned char* data, unsigned size, unsigned w, unsigned h);
        virtual void on_voip_window_set_offsets(void* hwnd, unsigned l, unsigned t, unsigned r, unsigned b);

        virtual void on_voip_device_changed(const std::string& dev_type, const std::string& uid);

        virtual void on_voip_switch_media(bool video);
        virtual void on_voip_volume_change(int vol);
        virtual void on_voip_mute_switch();
		virtual void on_voip_set_mute(bool mute);
        virtual void on_voip_mute_incoming_call_sounds(bool mute);
		virtual void on_voip_minimal_bandwidth_switch();


        virtual void post_voip_msg_to_server(const voip_manager::VoipProtoMsg& msg) = 0;
        virtual void post_voip_alloc_to_server(const std::string& data) = 0;

        virtual std::shared_ptr<wim::wim_packet> prepare_voip_msg(const std::string& data) = 0;

        virtual std::shared_ptr<wim::wim_packet> prepare_voip_pac(const voip_manager::VoipProtoMsg& data) = 0;

        virtual themes::theme* get_theme_from_cache(int _theme_id) = 0;
        virtual void get_theme(int64_t _seq, int _theme_id) = 0;

        // files functions
        virtual void upload_file_sharing(
            const int64_t _seq,
            const std::string& _contact,
            const std::string& _file_name,
            std::shared_ptr<core::tools::binary_stream> _data,
            const std::string& _extension) = 0;

        virtual void download_file_sharing(
            const int64_t _seq,
            const std::string& _contact,
            const std::string& _file_url,
            const std::string& _download_dir,
            const std::string& _filename,
            const file_sharing_function _function) = 0;

        virtual void request_file_direct_uri(
            const int64_t _seq,
            const std::string& _file_url) = 0;

        virtual void download_image(
            const int64_t _seq,
            const std::string& _contact_aimid,
            const std::string& _file_url,
            const std::string& _destination,
            const bool _download_preview,
            const int32_t _preview_width,
            const int32_t _preview_height) = 0;

        virtual void download_link_preview(
            const int64_t _seq,
            const std::string& _contact_aimid,
            const std::string& _url,
            const int32_t _preview_width,
            const int32_t _preview_height) = 0;

        virtual void cancel_loader_task(const int64_t _task_id) = 0;

        virtual void abort_file_sharing_download(
            const int64_t _seq,
            const int64_t _process_seq) = 0;
        virtual void abort_file_sharing_upload(
            const int64_t _seq,
            const std::string & _contact,
            const std::string &_process_seq) = 0;
        virtual void raise_download_priority(
            const int64_t _task_id) = 0;
        virtual void raise_contact_downloads_priority(
            const std::string &_contact_aimid) = 0;

        virtual void set_played(const std::string& url, bool played) = 0;
        virtual void speech_to_text(int64_t _seq, const std::string& _url, const std::string& _locale) = 0;

        // search for contacts
        virtual void search_contacts(int64_t _seq, const core::search_params& _filters) = 0;
        virtual void search_contacts2(int64_t _seq, const std::string& keyword, const std::string& phonenumber, const std::string& tag) = 0;
        virtual void get_profile(int64_t _seq, const std::string& _contact) = 0;

        // tools
        virtual void sign_url(int64_t _seq, const std::string& unsigned_url) = 0;
        virtual void load_flags(const int64_t _seq) = 0;

        virtual void update_profile(int64_t _seq, const std::vector<std::pair<std::string, std::string>>& _field) = 0;

        // live chats
        virtual void join_live_chat(int64_t _seq, const std::string& _stamp) = 0;

        virtual void set_avatar(const int64_t _seq, tools::binary_stream image, const std::string& _aimId) = 0;

        virtual void save_auth_to_export(std::function<void()> _on_result) = 0;
        virtual void set_show_promo_in_auth(bool _need_promo) = 0;
        virtual void start_after_close_promo() = 0;

        virtual void read_snap(const uint64_t _snap_id, const std::string& _aimId, const bool _mark_prev_snaps_read) = 0;
        virtual void download_snap_metainfo(const int64_t seq, const std::string& _contact_aimid, const std::string &ttl_id) = 0;
    };

}

#endif //#__BASE_IM_H_
