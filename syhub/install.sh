# Set variables
export INSTALL_DIR="/opt/syhub"
export SERVICE_FILE="/etc/systemd/system/syhub.service"

# Remove current install if it exists
if [ -d "$INSTALL_DIR" ]; then
    sudo rm -d -R "$INSTALL_DIR"
fi

# Remove service file if it exists
if [ -f "$SERVICE_FILE" ]; then
    sudo rm -f "$SERVICE_FILE"
fi

# Install repo to system
sudo cp -r ./ "$INSTALL_DIR"
sudo cp "syhub.service" "$SERVICE_FILE"

# Reload systemd
sudo systemctl daemon-reload

# Start service
sudo systemctl start syhub
sudo systemctl enable syhub
