include $(TOPDIR)/rules.mk

PKG_NAME:=openvpn_server_manager
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/openvpn_server_manager
	CATEGORY:=Base system
	TITLE:=openvpn_server_manager
	DEPENDS:= +libubus +libubox
endef

define Package/openvpn_server_manager/description
	This program can subscribe to multiple MQTT topics and react to events
endef

define Package/openvpn_server_manager/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/openvpn_server_manager $(1)/usr/bin
	$(INSTALL_BIN) ./files/openvpn_server_manager.init $(1)/etc/init.d/openvpn_server_manager
	$(INSTALL_CONF) ./files/openvpn_server_manager.config $(1)/etc/config/openvpn_server_manager
endef

$(eval $(call BuildPackage,openvpn_server_manager))
