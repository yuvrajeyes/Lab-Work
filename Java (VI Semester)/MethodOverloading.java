import java.util.*;

class Album {

    private final String name;
    private final String artist;
    private final List<Song> songs;

    public Album(String name, String artist) {
        this.name = name;
        this.artist = artist;
        this.songs = new ArrayList<>();
    }

    public String getName() {
        return name;
    }

    public String getArtist() {
        return artist;
    }

    public List<Song> getSongs() {
        return songs;
    }

    private Song findSong(String title) {
        for (Song song: this.songs) {
            if (song.getTitle().equals(title)) {
                return song;
            }
        }
        return null;
    }

    private Song findSong(Song song) {
        for (Song checkedSong: this.songs) {
            if (checkedSong.getTitle().equals(song.getTitle())) {
                return checkedSong;
            }
        }
        return null;
    }

    public void addSong(String title, double duration) {
        if (findSong(title) == null) {
            this.songs.add(new Song(title, duration));
            return;
        }
        System.out.println("This " + title + " is already in the " + getClass().getSimpleName());
    }

    public void addSong(Song song) {
        Song checkedSong = findSong(song);
        if (checkedSong == null) {
            songs.add(new Song(song.getTitle(), song.getDuration()));
            return;
        }
        System.out.println("This " + song.getTitle() + " is already in the " + getClass().getSimpleName());
    }

    public void addToPlaylist(String title, List<Song> playlist) {
        Song song = findSong(title);
        if (song != null) {
            for (Song checkedSong: playlist) {
                if (checkedSong.getTitle().equals(song.getTitle()) && checkedSong.getDuration() == song.getDuration()) {
                    System.out.println(song.getTitle() + ": " + song.getDuration() + " is already exist in the playlist");
                    return;
                }
            }
            playlist.add(song);
            return;
        }
        System.out.println("The song " + title + " is not in this album");
    }

    public void addToPlaylist(int trackNumber, List<Song> playlist) {
        int index = trackNumber - 1;
        if (index >= 0 && index < this.songs.size()) {
            Song song = this.songs.get(index);
            for (Song checkedSong: playlist) {
                if (checkedSong.getTitle().equals(song.getTitle()) && checkedSong.getDuration() == song.getDuration()) {
                    System.out.println("Track No. " + trackNumber + " (" + song.getTitle() + ": "
                            + song.getDuration() + ") is already exist in the playlist");
                    return;
                }
            }
            playlist.add(song);
            return;
        }
        System.out.println("This album does not have a track " + trackNumber);
    }


    class Song {

        private final String title;
        private final double duration;

        public Song(String title, double duration) {
            this.title = title;
            this.duration = duration;
        }

        public String getTitle() {
            return title;
        }

        public double getDuration() {
            return duration;
        }

        @Override
        public String toString() {
            return this.title + ": " + this.duration;
        }
    }
}

public class MethodOverloading {

    private static final List<Album> albums = new ArrayList<>();
    public static void main(String[] args) {

        Album album = new Album("Qismat 2", "Ammy Virk");
        album.addSong("Pagla", 4.25);
        album.addSong("Mere Yarra Ve", 4.24);
        album.addSong("Janam", 3.52);
        album.addSong("Qismat-2 Title Track", 4.59);
        album.addSong("Teri Akheeyan", 2.41);

        // Album.Song song = album.new Song("abc", 3.33);
        album.addSong(album.new Song("abc", 3.33));
        albums.add(album);

        LinkedList<Album.Song> playlist = new LinkedList<>();
        albums.get(0).addToPlaylist("Qismat-2 Title Track", playlist);
        albums.get(0).addToPlaylist(1, playlist);
        albums.get(0).addToPlaylist("Mere Yarra Ve", playlist);
        albums.get(0).addToPlaylist("Teri Akheeyan", playlist);
        albums.get(0).addToPlaylist("Janam", playlist);
        albums.get(0).addToPlaylist(8, playlist);
        albums.get(0).addToPlaylist("Qismat", playlist);
        albums.get(0).addToPlaylist("Janam", playlist);
        albums.get(0).addToPlaylist("abc", playlist);
        albums.get(0).addToPlaylist(1, playlist);
        System.out.println();
        play(playlist);
    }

    private static void play(List<Album.Song> playlist) {
        Scanner scanner = new Scanner(System.in);
        boolean quit = false;
        boolean forward = true;
        ListIterator<Album.Song> listIterator = playlist.listIterator();
        if (playlist.size() == 0) {
            System.out.println("There is no songs in playlist");
        } else {
            System.out.println("Now Playing " + listIterator.next());
            printMenu();
        }

        while(!quit) {
            int action = scanner.nextInt();
            scanner.nextLine();

            switch (action) {
                case 0:
                    System.out.println("Quiting playlist...");
                    quit = true;
                    break;
                case 1:
                    if(!forward) {
                        if(listIterator.hasNext()) {
                            listIterator.next();
                        }
                        forward = true;
                    }
                    if (listIterator.hasNext()) {
                        System.out.println("Now playing " + listIterator.next());
                    } else {
                        System.out.println("We have reached the end of the playlist");
                        forward = false;
                    }
                    break;
                case 2:
                    if (forward) {
                        if (listIterator.hasPrevious()) {
                            listIterator.previous();
                        }
                        forward = false;
                    }
                    if (listIterator.hasPrevious()) {
                        System.out.println("Now playing " + listIterator.previous());
                    } else {
                        System.out.println("We are at start of the playlist");
                        forward = true;
                    }
                    break;
                case 3:
                    if (forward) {
                        if(listIterator.hasPrevious()) {
                            System.out.println("Now replaying " + listIterator.previous());
                            forward = false;
                        } else {
                            System.out.println("We are at the start of the list");
                        }
                    } else {
                        if(listIterator.hasNext()) {
                            System.out.println("Now replaying " + listIterator.next());
                            forward = true;
                        } else {
                            System.out.println("We have reached the end of the list");
                        }
                    }
                    break;
                case 4:
                    printList(playlist);
                    break;
                case 5:
                    printMenu();
                    break;
                case 6:
                    if(playlist.size() > 0) {
                        listIterator.remove();
                        if (playlist.isEmpty()) {
                            System.out.println("All songs are removed from playlist.");
                        }
                        if(listIterator.hasNext()) {
                            System.out.println("Now playing " + listIterator.next());
                        } else if(listIterator.hasPrevious()) {
                            System.out.println("Now playing " + listIterator.previous());
                        }
                    } else {
                        System.out.println("Playlist is empty.");
                    }
                    break;
            }
        }
    }

    private static void printMenu() {
        System.out.println("Available actions:\npress");
        System.out.println("0 - to quit\n" +
                "1 - to play next song\n" +
                "2 - to play previous song\n" +
                "3 - to replay the current song\n" +
                "4 - list songs in the playlist\n" +
                "5 - print available actions.\n" +
                "6 - delete current song from playlist");

    }

    private static void printList(List<Album.Song> playList) {
        ListIterator<Album.Song> iterator = playList.listIterator();
        System.out.println("================================");
        while(iterator.hasNext()) {
            System.out.println(iterator.next());
        }
        System.out.println("================================");
    }
}
