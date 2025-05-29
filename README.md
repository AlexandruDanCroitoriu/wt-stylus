
https://redmine.emweb.be/boards/1/topics/18461?r=18466#message-18466
Wt designer can it run with the latest version of Wt?
https://redmine.emweb.be/boards/1/topics/15305?r=15434#message-15434
Wt designer dose not suport wt 4.0 or above





sh ./scripts/wt_builder.sh
sh ./scripts/wt_code_server.sh



tmux
mkdir work
cd work
git clone https://github.com/AlexandruDanCroitoriu/wt-stylus.git
cd wt-stylus
sh ./scripts/setup_vps.sh




alexandru-croitoriu.dev {
    root * /srv
    file_server
}
jenkins.alexandru-croitoriu.dev {
    reverse_proxy jenkins:8080
}
portainer.alexandru-croitoriu.dev {
    reverse_proxy portainer:9000
}
pgadmin.alexandru-croitoriu.dev {
    reverse_proxy pgadmin:8081
}
app-1.alexandru-croitoriu.dev {
    reverse_proxy app-1:9020
}